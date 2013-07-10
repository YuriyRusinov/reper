#if !defined(__KKSSITOOOM_KKSMap_h)
#define __KKSSITOOOM_KKSMap_h

#include "kkspp_config.h"
#include <vector>

#include <QMap>
#include <QtDebug>

template <typename Key, class T>
class /*_PP_EXPORT*/ KKSMap : public QMap<Key, T>
{
    public:
	KKSMap() : QMap<Key, T>() {}
        KKSMap(const KKSMap<Key, T> & other) : QMap<Key, T>(other)
        {
            for (typename QMap<Key, T>::const_iterator pl=this->constBegin(); pl != this->constEnd(); pl++)
                if (pl.value())
                    pl.value()->addRef();
        }
	
        ~KKSMap()
        {
            for (typename QMap<Key, T>::const_iterator pl=this->constBegin(); pl != this->constEnd(); pl++)
                if (pl.value())
                    pl.value()->release();
        }

        void clear()
        {
            for (typename QMap<Key, T>::const_iterator pl=this->constBegin(); pl != this->constEnd(); pl++)
                if (pl.value())
                    pl.value()->release();

            QMap<Key, T>::clear();
        }

        int insert(const Key & key, const T & p)
        {
            if(!p)
                return 0;

            if(this->count(key) != 0){
                qWarning() << "There are exist record in KKSMap with key = " << key;
                return 0;
            }

            QMap<Key, T>::insert(key, p);
            p->addRef();
            return 1;
        }
        
        int insertMulti(const Key & key, const T & p)
        {
            if(!p)
                return 0;

            QMap<Key, T>::insertMulti(key, p);
            p->addRef();
            return 1;
        }

        void remove(const Key & key)
        {
            QMap<Key, T> l = *this;
            QList<T> v = l.values(key);
            for (int i=0; i<v.count(); i++)
            {
                T p = v[i];
                if(p)
                    p->release();
            }

            QMap<Key, T>::remove(key);
        }
    
/*
    const T value(const Key & key) const
    {
        //const QMap<Key, T> l = *this;
		//QMap<Key, T>::value(key)
        return QMap<Key, T>::value(key);
    }
	*/
	/*
    const T operator[](const Key & key) const
    {
        //const QMap<Key, T> l = *this;
        //T p = ;

        return (QMap<Key, T>(*this))[key];
    }
	*/

    /*
	T & operator[](const Key & key)
    {
        //QMap<Key, T> l = *this;
        //T p = l[key];

        return (*this)[key];
    }
	*/
        KKSMap<Key, T> & operator = ( const KKSMap<Key, T> & cp )
        {
            if ( this == &cp )
                return *this;

            QMap<Key, T> l = *this;
            for (typename QMap<Key, T>::const_iterator pl=this->constBegin(); pl != this->constEnd(); pl++)
                if (pl.value())
                    pl.value()->release();
	    
            QMap<Key, T> * t = this;
            *t = cp;

            l = *this;
            for (typename QMap<Key, T>::const_iterator pl=this->constBegin(); pl != this->constEnd(); pl++)
                if (pl.value())
                    pl.value()->addRef();

            return *this;
        }
#ifdef Q_WS_WIN
        //for web
		std::map<Key, T> toStdMp() const
        {
            return QMap<Key,T>::toStdMap();
        }
		std::vector<Key> keys_w() const
		{
			QList<Key> keys = QMap<Key,T>::keys();
			std::vector<Key> b;
			for (int i = 0; i < keys.size(); ++i) {
             b.push_back(keys.at(i));
         }
		 return b;
		}
		std::vector<T> values_w() const
		{
			QList<T> values = QMap<Key,T>::values();
			std::vector<T> b;
			for (int i = 0; i < values.size(); ++i) {
             b.push_back(values.at(i));
         }
		 return b;
		}
		#endif

};

#endif
