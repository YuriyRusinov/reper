/* 
 * File:   KKSAttrHistModel.cpp
 * Author: rusinov
 * 
 * Created on 30 Август 2013 г., 16:16
 */

#include <KKSAttrValue.h>
#include "KKSAttrHistModel.h"

KKSAttrHistModel::KKSAttrHistModel(const KKSList<KKSAttrValue*>& _histlist, QObject * parent)
    : QAbstractItemModel (parent),
      histList (_histlist)
{
}

KKSAttrHistModel::~KKSAttrHistModel()
{
}

int KKSAttrHistModel::columnCount (const QModelIndex& parent) const
{
    
}

int KKSAttrHistModel::rowCount (const QModelIndex& parent) const
{
    
}

QModelIndex KKSAttrHistModel::index (int row, int column, const QModelIndex& parent) const
{
    
}

QModelIndex KKSAttrHistModel::parent (const QModelIndex& index) const
{
}

Qt::ItemFlags KKSAttrHistModel::flags (const QModelIndex& index) const
{
}

QVariant KKSAttrHistModel::data (const QModelIndex& index, int role) const
{
}

QVariant KKSAttrHistModel::headerData (int section, Qt::Orientation orientation, int role) const
{
}