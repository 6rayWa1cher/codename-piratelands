#ifndef ENEMYSTATSMODEL_H
#define ENEMYSTATSMODEL_H

#include <QAbstractTableModel>
#include <memory>
#include "hero.h"

class EnemyStatsModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    std::shared_ptr<Enemy> _enemy;
    bool _hints;
public:
    EnemyStatsModel(QObject *parent = nullptr, std::shared_ptr<Enemy> enemy = nullptr, bool hits = true);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setEnemy(const std::shared_ptr<Enemy> &enemy);

private:

public slots:
    void replaceTeam(std::shared_ptr<ShipBoardingTeam> team);
    void replaceCannons(std::shared_ptr<ShipCannons> cannons);
    void replaceHull(std::shared_ptr<ShipHull> hull);
    void replaceSail(std::shared_ptr<ShipSail> sail);
};

#endif // ENEMYSTATSMODEL_H
