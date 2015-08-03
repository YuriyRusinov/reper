#ifndef KKSCMDJOURNALLV_H
#define KKSCMDJOURNALLV_H

#include <QTreeView>
#include <QWidget>
#include <QAbstractItemModel>

#include "kkscmdjournalitem.h"
#include "kksgui_config.h"


//class FBLvUrlsFilter;
//class FBSettings;

class _GUI_EXPORT KKSCmdJournalModel : public QAbstractItemModel
{
	public:

        enum SortType
        {
            byInsertTime = 0,
            byExecTime = 1
        };

        KKSCmdJournalModel( //const FBSettings * pSettings, 
                      const QList< KKSCmdJournalItemData * > &commands, 
                      //bool bFavourites,
                      QObject * parent = 0 );
        KKSCmdJournalModel( //const FBSettings * pSettings, 
                      //bool bFavourites,
                      QObject * parent = 0 );
        ~KKSCmdJournalModel( void );

        virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
        virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
        virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;

        virtual bool insertRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
        virtual bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
        
        QModelIndex index(int row, int column,
                          const QModelIndex &parent = QModelIndex()) const;
        QModelIndex parent(const QModelIndex &index) const;

        
        Qt::ItemFlags flags(const QModelIndex &index) const;
        virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        //const FBSettings * settings() const {return pSettings;}
        //bool isForFavourites() const { return m_bFavourites; }

        //void startAnalyze();
        
        void sort(SortType type, bool asc);

public slots:
    void slotDataChanged(int row);

signals:
    void dataChangedEx(const KKSCommand & cmd);

private:
        
        void initHeaderData();

        QList< KKSCmdJournalItemData * > m_commands;

        int ncols;
        QStringList header;
        //const FBSettings * pSettings;
        //bool m_bFavourites;

    private:
        Q_OBJECT
};


class _GUI_EXPORT KKSCmdJournalLv : public QTreeView
{
    Q_OBJECT
public:
    KKSCmdJournalLv(//const FBSettings* pSettings, 
                    //QString phrase, 
                    //bool bFavourites = false, 
                    QWidget * parent = 0);
    ~KKSCmdJournalLv();

    //FBLvUrlsFilter * getFilter(){return filter;}
    
    //const QString & getPhrase() const {return phrase;}
    //void setPhrase(const QString & in){phrase = in;}
    KKSCommand currentCommand() const;

    void setMenu(QMenu * menu){m_pMenu = menu;}

    int rowCount() const;
    bool isEmpty() const;

    void clear();

public slots:
    void addCommand(const KKSCommand & cmd);
    void removeCommand(const KKSCommand & cmd);
    void removeCommand(const QModelIndex & index);
    //void filterUrls();

    void sortByColumnEx(int index);

    void slotDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
    //void slotSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
    //void slotActivated(const QModelIndex& index);


    void slotLayoutChanged()
    {
        repaint();
    }

signals:
    void focusIn();
    void focusOut();
    void menuStarted();
    void menuStopped();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void mouseMoveEvent(QMouseEvent * e);
    virtual void focusInEvent(QFocusEvent * e);
    virtual void focusOutEvent(QFocusEvent * e);

private:
    
    //FBLvUrlsFilter * filter;

    //QString phrase;
    //bool m_bFavourites;
    QMenu * m_pMenu;
};

#endif
