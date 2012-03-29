#include "renderobjects.h"
#include "orutils.h"
#include "barcodes.h"
#include "graph.h"
#include "crosstab.h"
#include <QtDebug>

#include "KKSPrerender.h"
class ORPreRenderPrivate {
  public:
    ORPreRenderPrivate();
    virtual ~ORPreRenderPrivate();

    bool _valid;
    QDomDocument _docReport;
    ParameterList _lstParameters;

    QSqlDatabase _database;
    ORODocument* _document;
    OROPage*     _page;
    ORReportData* _reportData;

    qreal _yOffset;      // how far down the current page are we in inches. That way we can use dpi.
    qreal _topMargin;    // value stored in the correct units
    qreal _bottomMargin; // -- same as above --
    qreal _leftMargin;   // -- same as above --
    qreal _rightMargin;  // -- same as above --
    qreal _maxHeight;    // -- same as above --
    qreal _maxWidth;     // -- same as above --
    int _pageCounter;    // what page are we currently on?

    QList<orQuery*> _lstQueries;
    QMap<QString, QColor> _colorMap;
    QList<OROTextBox*> _postProcText;

    // data for the watermark feature
    bool    _wmStatic;   // is this watermark static text or is the data
                         // pulled from a query source
    QString _wmText;     // the text that is to be rendered as a watermark
    ORDataData _wmData;  // the dynamic source for the watermark text
    QFont   _wmFont;     // the font to use when rendering the _wmText value
                         // on the background of the page. For purposes of
                         // fit, pointSize() of font is ignored and the largest
                         // point size is used that will still allow the entire
                         // text to fit on the page.
    unsigned char _wmOpacity; // a value from 0..255
                              // with 0   being not visible (white)
                              // and  255 being fully opaque (black)

    bool    _bgStatic;   // is this image static or pulled from a datasource
    QImage  _bgImage;    // the image that is to be rendered as a background
    ORDataData _bgData;  // the dynamic source for the background image
    QRectF  _bgRect;     // area of the page that we print background to
    unsigned char _bgOpacity; // a value from 0..255 : see _wmOpacity
    int     _bgAlign;    // Alignment of background within _bgRect
    bool    _bgScale;    // scale image to fit in _bgRect?
    Qt::AspectRatioMode _bgScaleMode; // how do we scale the image?

    void renderBackground(OROPage *);
    void renderWatermark(OROPage *);

    QMap<ORDataData,double> _subtotPageCheckPoints;
    QMap<ORDataData,double> * _subtotContextMap;
    ORDetailSectionData * _subtotContextDetail;
    bool _subtotContextPageFooter;

    bool populateData(const ORDataData &, orData &);
    orQuery* getQuerySource(const QString &);

    void createNewPage();
    qreal finishCurPage(bool = false);
    qreal finishCurPageSize(bool = false);

    void renderDetailSection(ORDetailSectionData &);
    qreal renderSection(const ORSectionData &);
    qreal renderSectionSize(const ORSectionData &);

    // Calculate the remaining space on the page after printing the footers and applying the margins
    qreal calculateRemainingPageSize(bool lastPage = false);

    double getNearestSubTotalCheckPoint(const ORDataData &);
};

KKSPrerender :: KKSPrerender (void)
    : ORPreRender ()
{
}

KKSPrerender :: KKSPrerender (const QDomDocument & doc)
    : ORPreRender (doc)
{
}

KKSPrerender :: KKSPrerender (const QDomDocument& doc, const ParameterList& params)
    : ORPreRender (doc, params)
{
}

KKSPrerender :: ~KKSPrerender (void)
{
}

ORODocument * KKSPrerender :: generate ()
{
    qDebug () << __PRETTY_FUNCTION__;
    QuerySource * qs = 0;
    QStringList sqlQueries;
    for(unsigned int i = 0; i < _internal->_reportData->queries.size(); i++)
    {
        qs = _internal->_reportData->queries.get(i);
        qDebug () << __PRETTY_FUNCTION__ << qs->name() << qs->query(_internal->_database);
        sqlQueries.append (qs->query(_internal->_database));
        _internal->_lstQueries.append(new orQuery(qs->name(), qs->query(_internal->_database), _internal->_lstParameters, true, _internal->_database));
    }
/*   _internal->createNewPage();
    if(_internal->_reportData->rpthead != 0)
      _internal->renderSection(*(_internal->_reportData->rpthead));

    for(int i = 0; i < _internal->_reportData->sections.count(); i++)
      if(_internal->_reportData->sections.at(i) != 0)
        _internal->renderDetailSection(*(_internal->_reportData->sections.at(i)));

    if(_internal->_reportData->rptfoot != 0)
    {
      if ( _internal->renderSectionSize(*(_internal->_reportData->rptfoot)) + _internal->finishCurPageSize(true)
         + _internal->_bottomMargin + _internal->_yOffset >= _internal->_maxHeight)
        _internal->createNewPage();
      _internal->renderSection(*(_internal->_reportData->rptfoot));
    }
*/

    return 0;
}
