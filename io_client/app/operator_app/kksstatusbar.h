#ifndef KKSStatusBar_H
#define KKSStatusBar_H

#include <QStatusBar>
#include <QWidget>
#include <QObject>
#include <QPixmap>
#include <QLabel>

/*!\brief Вспомогательный класс, отвечающий за отрисовку ячейки индикатора на статусбаре.

Содержит QPixmap и QLabel.
Пример использования:
\code
QPixmap px = ...;
QString text = ...;
QWidget * parent = ...;
CellWidget * w = new CellWidget(px, text, parent);

...//при необходимости обновить текст индикатора
w->setLabelText("100");
\endcode
*/
class CellWidget : public QWidget
{
Q_OBJECT
public:
    CellWidget(const QPixmap & _px, const QString & pText, const QString & text, QWidget * parent = NULL);
    virtual ~CellWidget();
    
    void setPixmap(const QPixmap & _px);
    void setPersistentText(const QString & text);
    void setText(const QString & text);

    const QPixmap & pixmap() const;
    const QString & text() const;
    const QString & persistentText() const;
    QLabel * label() const;

private:
    QPixmap px;
    QLabel * m_label;
    QString m_pText;
    QString m_text;
};

class KKSStatusBar : public QStatusBar
{
    Q_OBJECT

public:
    KKSStatusBar(const QString & userName, 
                 const QString & dlName,
                 QWidget * parent = 0);
    virtual ~KKSStatusBar();

    void setHellow(const QPixmap & px, 
                   const QString & userName, 
                   const QString & dlName);

    void showStatusBar();
    
    void updateHellow(const QString & userName, 
                      const QString & dlName);

private:
    void init();
    
    QString m_userName;
    QString m_dlName;

    CellWidget * cwHellow;
};


#endif
