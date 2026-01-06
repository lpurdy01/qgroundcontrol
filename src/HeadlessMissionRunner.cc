#include "HeadlessMissionRunner.h"

#include <QtCore/QFileInfo>

#include "FirmwarePlugin/FirmwarePlugin.h"
#include "MissionManager/MissionManager.h"
#include "MissionManager/PlanManager.h"
#include "MissionManager/PlanMasterController.h"
#include "Vehicle/MultiVehicleManager.h"
#include "Vehicle/Vehicle.h"

Q_LOGGING_CATEGORY(HeadlessMissionRunnerLog, "HeadlessMissionRunner")

HeadlessMissionRunner::HeadlessMissionRunner(const QString& planFile, QObject* parent)
    : QObject(parent)
    , _planFile(QFileInfo(planFile).absoluteFilePath())
{
    const QFileInfo info(_planFile);
    if (!info.exists()) {
        qCWarning(HeadlessMissionRunnerLog) << "Mission plan not found" << info.filePath();
        return;
    }

    MultiVehicleManager* manager = MultiVehicleManager::instance();
    if (!manager) {
        qCWarning(HeadlessMissionRunnerLog) << "MultiVehicleManager unavailable";
        return;
    }

    connect(manager, &MultiVehicleManager::activeVehicleAvailableChanged,
            this, &HeadlessMissionRunner::_onActiveVehicleAvailable);
    connect(manager, &MultiVehicleManager::activeVehicleChanged,
            this, &HeadlessMissionRunner::_handleActiveVehicle);

    _handleActiveVehicle(manager->activeVehicle());
}

void HeadlessMissionRunner::_onActiveVehicleAvailable(bool available)
{
    if (!available) {
        _detachVehicle();
        return;
    }

    if (!_vehicle) {
        if (MultiVehicleManager* manager = MultiVehicleManager::instance()) {
            _handleActiveVehicle(manager->activeVehicle());
        }
    }
}

void HeadlessMissionRunner::_handleActiveVehicle(Vehicle* vehicle)
{
    if (vehicle == _vehicle) {
        return;
    }

    _detachVehicle();

    if (!vehicle) {
        return;
    }

    _vehicle = vehicle;

    MissionManager* missionManager = _vehicle->missionManager();
    if (!missionManager) {
        qCWarning(HeadlessMissionRunnerLog) << "Mission manager unavailable for vehicle" << _vehicle->id();
        _vehicle = nullptr;
        return;
    }

    connect(missionManager, &PlanManager::sendComplete,
            this, &HeadlessMissionRunner::_missionSendComplete);
    connect(missionManager, &PlanManager::error,
            this, &HeadlessMissionRunner::_missionError);

    connect(_vehicle, &Vehicle::armedChanged,
            this, [](bool armed) {
                qCInfo(HeadlessMissionRunnerLog) << "Vehicle armed state" << armed;
            });

    _beginMissionUpload();
}

void HeadlessMissionRunner::_detachVehicle()
{
    if (!_vehicle) {
        return;
    }

    if (MissionManager* missionManager = _vehicle->missionManager()) {
        missionManager->disconnect(this);
    }

    _vehicle->disconnect(this);
    _vehicle = nullptr;
    _uploadInProgress = false;
    _missionUploaded = false;
    _missionStarted = false;
}

void HeadlessMissionRunner::_beginMissionUpload()
{
    if (!_vehicle || _uploadInProgress || _missionUploaded) {
        return;
    }

    qCInfo(HeadlessMissionRunnerLog) << "Uploading mission plan" << _planFile;
    _uploadInProgress = true;
    PlanMasterController::sendPlanToVehicle(_vehicle, _planFile);
}

void HeadlessMissionRunner::_missionSendComplete(bool error)
{
    if (!_uploadInProgress) {
        return;
    }

    _uploadInProgress = false;

    if (error) {
        qCWarning(HeadlessMissionRunnerLog) << "Mission upload failed";
        return;
    }

    _missionUploaded = true;
    qCInfo(HeadlessMissionRunnerLog) << "Mission upload complete";
    _startMission();
}

void HeadlessMissionRunner::_missionError(int errorCode, const QString& errorMsg)
{
    Q_UNUSED(errorCode)
    qCWarning(HeadlessMissionRunnerLog) << "Mission manager error" << errorMsg;
}

void HeadlessMissionRunner::_startMission()
{
    if (!_vehicle || _missionStarted || !_missionUploaded) {
        return;
    }

    FirmwarePlugin* plugin = _vehicle->firmwarePlugin();
    if (!plugin) {
        qCWarning(HeadlessMissionRunnerLog) << "Firmware plugin unavailable";
        return;
    }

    qCInfo(HeadlessMissionRunnerLog) << "Starting mission";
    plugin->startMission(_vehicle);
    _missionStarted = true;
}
