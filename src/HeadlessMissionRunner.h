#pragma once

#include <QtCore/QLoggingCategory>
#include <QtCore/QObject>
#include <QtCore/QString>

class Vehicle;

Q_DECLARE_LOGGING_CATEGORY(HeadlessMissionRunnerLog)

class HeadlessMissionRunner : public QObject
{
    Q_OBJECT
public:
    explicit HeadlessMissionRunner(const QString& planFile, QObject* parent = nullptr);

private slots:
    void _onActiveVehicleAvailable(bool available);
    void _handleActiveVehicle(Vehicle* vehicle);
    void _missionSendComplete(bool error);
    void _missionError(int errorCode, const QString& errorMsg);

private:
    void _detachVehicle();
    void _beginMissionUpload();
    void _startMission();

    QString _planFile;
    Vehicle* _vehicle = nullptr;
    bool _uploadInProgress = false;
    bool _missionUploaded = false;
    bool _missionStarted = false;
};
