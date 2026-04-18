#ifndef APIEXTERNALTOOL_H
#define APIEXTERNALTOOL_H
#include "externaltoolbase.h"

class APIExternalTool : public ExternalToolBase
{
public:
    APIExternalTool(QObject *parent = nullptr);
    ~APIExternalTool();

    void SetArguments(const QStringList &args) override;
    void Run() override;
    void Stop() override;
    bool IsRunning() const override;
    QString GetOutput() override;

private:
    void Initialize();

};

#endif