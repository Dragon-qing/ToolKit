#include "apiexternaltool.h"

APIExternalTool::APIExternalTool(QObject *parent)
    : ExternalToolBase(parent)
{

}

APIExternalTool::~APIExternalTool()
{

}

void APIExternalTool::SetArguments(const QStringList &args)
{
    Q_UNUSED(args)
}

void APIExternalTool::Run()
{
}

void APIExternalTool::Stop()
{
}

bool APIExternalTool::IsRunning() const
{
    return false;
}

QString APIExternalTool::GetOutput()
{
    return QString();
}

void APIExternalTool::Initialize()
{
}
