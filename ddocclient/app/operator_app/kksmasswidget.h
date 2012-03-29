#ifndef FBLOG_MassWidget_H
#define FBLOG_MassWidget_H

#include <QWidget>

class QCheckBox;
class QSpinBox;

class MassWidget : public QWidget
{
    public:
        MassWidget (QWidget *parent=0, Qt::WindowFlags f=0);
        virtual ~MassWidget (void);

        bool isMass (void) const;
        int num (void) const;

    private slots:
        void setMass (int state);

    private:
        //
        // Variables
        //
        QCheckBox * chMass;
        QSpinBox *sbNum;

    private:
        Q_OBJECT
};

#endif
