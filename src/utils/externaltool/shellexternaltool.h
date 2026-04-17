#ifndef SHELLEXTERNALTOOL_H
#define SHELLEXTERNALTOOL_H
#include "externaltoolbase.h"

class ShellExternalTool : public ExternalToolBase
{
public:
    ShellExternalTool();
    ~ShellExternalTool();

    // ExternalToolBase interface
public:
    void SetArguments(const QStringList &args) override;
    void Run() override;
    void Stop() override;
    bool IsRunning() const override;
    QString GetOutput() override;

private:
    void Initialize();
};

#endif