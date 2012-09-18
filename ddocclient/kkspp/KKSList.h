#if !defined(__KKSSITOOOM_KKSList_h)
#define __KKSSITOOOM_KKSList_h

#include "kkspp_config.h"
//#include "KKSData.h"

#include <QList>

#ifdef Q_CC_MSVC
#pragma warning(disable : 4661)//за комментариями к ksa
#endif

template <class T>
class /*_PP_EXPORT*/ KKSList : public QList<T>
{
    public:
        KKSList(){}
        KKSList(const KKSList<T> & other) : QList<T>(other)
        {
            //QList<T> l = *this;
            for (typename QList<T>::const_iterator pl=this->constBegin(); pl != this->constEnd(); pl++)
                if (*pl)
                    (*pl)->addRef();
        }

        ~KKSList()
        {
            QList<T> l = *this;
            for (typename QList<T>::const_iterator pl=this->constBegin(); pl != this->constEnd(); pl++)
                if (*pl)
                    (*pl)->release();
        }


        void append(const T & p)
        {
            if(!p)
                return;

            QList<T>::append(p);
            p->addRef();
        }
        
        void insert(int i, const T & p)
        {
            if(!p)
                return;

            QList<T>::insert(i, p);
            p->addRef();
        }
    
        int removeAt(int i)
        {
            if(i >= QList<T>::count())
                return 0;

            QList<T> l = *this;
            T p = l[i];
            if(p)
                p->release();

            QList<T>::removeAt(i);
            return 1;
        }
    
        int removeAll(const T & t)
        {
            QList<T> l = *this;
            int count = l.count();
            int removedCount = 0;
            for(int index=0; index<count; index++)
            {
                T p = (*this)[index];
                if(p == t)
                {
                    removeAt(index);
                    index--;
                    count--;
                    removedCount++;
                    //if(p)
                    //    p->release();
                }
            }

        return removedCount;
    }
/*
    const T & at(int i) const
    {
        //T p = QList<T>::at(i);

        return QList<T>::at(i);
    }

    const T & operator[](int i) const
    {
        const QList<T> l = *this;
        //T p = ;

        return l[i];
    }

    T & operator[](int i)
    {
        QList<T> l = *this;
        //T p = l[i];

        return l[i];
    }
*/

        KKSList<T> & operator = ( const KKSList<T> & cp )
        {
            if ( this == &cp )
                return *this;

            for (typename QList<T>::const_iterator pl=this->constBegin(); pl != this->constEnd(); pl++)
                if (*pl)
                    (*pl)->release();

            QList<T> * t = this;
            *t = cp;

            for (typename QList<T>::const_iterator pl=this->constBegin(); pl != this->constEnd(); pl++)
                if (*pl)
                    (*pl)->addRef();

            return *this;
        }

        void clear()
        {
            for (typename QList<T>::const_iterator pl=this->constBegin(); pl != this->constEnd(); pl++)
                if (*pl)
                    (*pl)->release();

            QList<T>::clear();
        }

		#ifdef Q_WS_WIN
		//for web 

        const T  at_w(int i) const
        {
            T p = QList<T>::at(i);

            return p;
        }

        int size_w(void) const
        {
            return QList<T>::size();
        }
		#endif


//private:
    //QVector<T> toVector() const { QVector<T> v; return v;}
};

#endif
