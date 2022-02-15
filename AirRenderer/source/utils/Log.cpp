#include "include/utils/Log.h"
#include <qdebug.h>

void Log::LogMatrix(std::string name, glm::mat4 matrix)
{
    qDebug() << QString::fromStdString("Matrix " + name + " :");
    for (int i = 0; i < 4; i++)
    {
        QString s = "";
        for (int j = 0; j < 4; j++)
        {
            s += QString::number(matrix[j][i], 'f', 6);
            s += " ";
        }
        qDebug() << s;
    }
}
