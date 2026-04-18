#include "shellexternaltool.h"

ShellExternalTool::ShellExternalTool()
{

}

ShellExternalTool::~ShellExternalTool()
{

}

void ShellExternalTool::SetArguments(const QStringList &args)
{
    Q_UNUSED(args)
}

void ShellExternalTool::Run()
{
}

void ShellExternalTool::Stop()
{
}

bool ShellExternalTool::IsRunning() const
{
    return false;
}

QString ShellExternalTool::GetOutput()
{
    return QString();
}

void ShellExternalTool::Initialize()
{
}