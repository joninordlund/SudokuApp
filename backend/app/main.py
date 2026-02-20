import uuid
import os
import time
from fastapi import UploadFile, File, HTTPException, FastAPI, BackgroundTasks
from fastapi.staticfiles import StaticFiles
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import FileResponse

UPLOAD_DIR = "uploads"
REACT_BUILD_DIR = "../frontend/dist"
os.makedirs(UPLOAD_DIR, exist_ok=True)

file_registry = {}
EXPIRY_SECONDS = 300


app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # POC-vaiheessa kaikki sallittu, tuotannossa rajoita
    allow_methods=["*"],
    allow_headers=["*"],
)


@app.post("/upload")
async def upload_file(file: UploadFile = File(...)):
    file_id = str(uuid.uuid4())
    file_path = os.path.join(UPLOAD_DIR, file_id)

    content = await file.read()
    with open(file_path, "wb") as f:
        f.write(content)

    file_registry[file_id] = {
        "path": file_path,
        "created_at": time.time(),
        "used": False,
    }

    return {
        "download_url": f"http://localhost:8000/download/{file_id}",
        "expires_in": EXPIRY_SECONDS,
    }


@app.get("/download/{file_id}")
def download_file(file_id: str, background_tasks: BackgroundTasks):

    data = file_registry.get(file_id)

    if not data:
        raise HTTPException(status_code=404, detail="Not found")

    if data["used"]:
        raise HTTPException(status_code=404, detail="Already used")

    if time.time() - data["created_at"] > EXPIRY_SECONDS:
        raise HTTPException(status_code=404, detail="Expired")

    if not os.path.exists(data["path"]):
        raise HTTPException(status_code=404, detail="File missing")

    data["used"] = True

    background_tasks.add_task(os.remove, data["path"])

    return FileResponse(data["path"])


if os.path.exists(REACT_BUILD_DIR):
    app.mount(
        "/assets", StaticFiles(directory=f"{REACT_BUILD_DIR}/assets"), name="static"
    )

    # 2. Kaikki muut reitit ohjataan Reactin index.html:ään (React Router hoitaa loput)
    @app.get("/{full_path:path}")
    async def serve_react(full_path: str):
        # Jos pyyntö ei ala /api/, tarjoillaan index.html
        return FileResponse(f"{REACT_BUILD_DIR}/index.html")
