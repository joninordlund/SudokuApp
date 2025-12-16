#include "history.h"
#include "grid.h"

#include <qdebug.h>

void History::undo()
{
    qDebug() << "Undo past size: " << m_past.size();
    qDebug() << "Undo future size: " << m_future.size();

    if (m_past.size() > 0)
    {
        vector<CellChange> item;
        item = m_past.back();
        for (const auto& cc : item)
        {
            m_grid->applyStateChange(cc.x, cc.y, cc.oldState.digit);
        }
        m_future.push_back(item);
        m_past.pop_back();
    }
}

void History::redo()
{
    qDebug() << "Redo past size: " << m_past.size();
    qDebug() << "Redo future size: " << m_future.size();

    if (m_future.size() > 0)
    {
        vector<CellChange> item;
        item = m_future.back();
        for (const auto& cc : item)
        {
            m_grid->applyStateChange(cc.x, cc.y, cc.newState.digit);
        }
        m_past.push_back(item);
        m_future.pop_back();
    }
}

void History::setNewCommand(vector<CellChange> cmd)
{
    m_past.push_back(cmd);
    m_future.clear();
    qDebug() << "NewCommand Past size: " << m_past.size();
}
