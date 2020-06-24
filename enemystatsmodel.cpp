#include "enemystatsmodel.h"
#include "item.h"

#include <QSize>

void EnemyStatsModel::setEnemy(const std::shared_ptr<Enemy> &enemy) {
    if (enemy != _enemy) {
        QObject::disconnect(&*_enemy, SIGNAL(team_changed(std::shared_ptr<ShipBoardingTeam>)), this, SLOT(replaceTeam(std::shared_ptr<ShipBoardingTeam>)));
        QObject::disconnect(&*_enemy, SIGNAL(cannons_changed(std::shared_ptr<ShipCannons>)), this, SLOT(replaceCannons(std::shared_ptr<ShipCannons>)));
        QObject::disconnect(&*_enemy, SIGNAL(hull_changed(std::shared_ptr<ShipHull>)), this, SLOT(replaceHull(std::shared_ptr<ShipHull>)));
        QObject::disconnect(&*_enemy, SIGNAL(sail_changed(std::shared_ptr<ShipSail>)), this, SLOT(replaceSail(std::shared_ptr<ShipSail>)));
        _enemy = enemy;
        emit modelReset({});
    }
    QObject::connect(&*enemy, SIGNAL(team_changed(std::shared_ptr<ShipBoardingTeam>)), this, SLOT(replaceTeam(std::shared_ptr<ShipBoardingTeam>)));
    QObject::connect(&*enemy, SIGNAL(cannons_changed(std::shared_ptr<ShipCannons>)), this, SLOT(replaceCannons(std::shared_ptr<ShipCannons>)));
    QObject::connect(&*enemy, SIGNAL(hull_changed(std::shared_ptr<ShipHull>)), this, SLOT(replaceHull(std::shared_ptr<ShipHull>)));
    QObject::connect(&*enemy, SIGNAL(sail_changed(std::shared_ptr<ShipSail>)), this, SLOT(replaceSail(std::shared_ptr<ShipSail>)));
}

void EnemyStatsModel::replaceTeam(std::shared_ptr<ShipBoardingTeam> /*team*/) {
    emit dataChanged(createIndex(3,0), createIndex(3,1));
}

void EnemyStatsModel::replaceCannons(std::shared_ptr<ShipCannons> /*cannons*/) {
    emit dataChanged(createIndex(0,0), createIndex(0,1));
}

void EnemyStatsModel::replaceHull(std::shared_ptr<ShipHull> /*hull*/) {
    emit dataChanged(createIndex(1,0), createIndex(1,1));
}

void EnemyStatsModel::replaceSail(std::shared_ptr<ShipSail> /*sail*/) {
    emit dataChanged(createIndex(2,0), createIndex(2,1));
}

EnemyStatsModel::EnemyStatsModel(QObject *parent, std::shared_ptr<Enemy> enemy, bool hints)
    : QAbstractTableModel(parent), _enemy(enemy), _hints(hints)
{
    setEnemy(enemy);
}

QVariant EnemyStatsModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Слот");
        case 1:
            return QString("Предмет");
        }
    }
    return QVariant();
}

int EnemyStatsModel::rowCount(const QModelIndex &/*parent*/) const {
    return 4;
}

int EnemyStatsModel::columnCount(const QModelIndex &/*parent*/) const {
    return 2;
}

QVariant EnemyStatsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (index.row()) {
        case 0:
            if (index.column() == 0){
                return QString("Пушки");
            } else {
                return _enemy->cannons()->name;
            }
        case 1:
            if (index.column() == 0){
                return QString("Корпус");
            } else {
                return _enemy->hull()->name;
            }
        case 2:
            if (index.column() == 0){
                return QString("Мачты");
            } else {
                return _enemy->sail()->name;
            }
        case 3:
            if (index.column() == 0){
                return QString("Команда");
            } else {
                return _enemy->team()->name;
            }
        default:
            return QVariant();
        }
    }
    if (_hints && role == Qt::ToolTipRole) {
        switch (index.row()) {
        case 0:
            return _enemy->cannons()->description;
        case 1:
            return _enemy->hull()->description;
        case 2:
            return _enemy->sail()->description;
        case 3:
            return _enemy->team()->description;
        default:
            return QVariant();
        }
    }
    return QVariant();
}
