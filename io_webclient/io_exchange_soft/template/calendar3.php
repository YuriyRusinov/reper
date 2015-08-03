<div id="yahoo-com3" class="yui-skin-eco">
    <div id="quarter">
    </div>
</div>

<script language="JavaScript" type="text/javascript">
YAHOO.namespace("yui.quarter");

YAHOO.yui.quarter.init = function() {
	YAHOO.yui.quarter.cal3 = new YAHOO.widget.CalendarGroup("cal3","quarter",{PAGES:3,START_WEEKDAY: 1,HIDE_BLANK_WEEKS:true});


	YAHOO.yui.quarter.cal3.cfg.setProperty("DATE_FIELD_DELIMITER", ".");
	YAHOO.yui.quarter.cal3.cfg.setProperty("MDY_DAY_POSITION", 1);
	YAHOO.yui.quarter.cal3.cfg.setProperty("MDY_MONTH_POSITION", 2);
	YAHOO.yui.quarter.cal3.cfg.setProperty("MDY_YEAR_POSITION", 3);

	YAHOO.yui.quarter.cal3.cfg.setProperty("MD_DAY_POSITION", 1);
	YAHOO.yui.quarter.cal3.cfg.setProperty("MD_MONTH_POSITION", 2);

	YAHOO.yui.quarter.cal3.cfg.setProperty("MONTHS_SHORT",   ["Янв", "Фев", "Март", "Апр", "Май", "Июн", "Июл", "Авг", "Сен", "Окт", "Ноя", "Дек"]);
	YAHOO.yui.quarter.cal3.cfg.setProperty("MONTHS_LONG",    ["Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"]);
	YAHOO.yui.quarter.cal3.cfg.setProperty("WEEKDAYS_1CHAR", ["В", "П", "В", "С", "Ч", "П", "С"]);
	YAHOO.yui.quarter.cal3.cfg.setProperty("WEEKDAYS_SHORT", ["вс", "пн", "вт", "ср", "чт", "пт", "сб"]);
	YAHOO.yui.quarter.cal3.cfg.setProperty("WEEKDAYS_MEDIUM",["Вос", "Пон", "Вт", "Ср", "Чет", "Пят", "Суб"]);
	YAHOO.yui.quarter.cal3.cfg.setProperty("WEEKDAYS_LONG",  ["Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"]);
    
	YAHOO.yui.quarter.cal3.cfg.setProperty("PAGEDATE", "<?=date('m.Y',mktime(0, 0, 0, date('m'), 0,   date('Y')))?>");
	 
	YAHOO.yui.quarter.cal3.render();

    YAHOO.yui.quarter.selectDate = function (p_sType, p_aArgs) {
        var aDate;
        var sDate;

        if (p_aArgs) {
            aDate = p_aArgs[0][0];
            var year = aDate[0], month = aDate[1], day = aDate[2];
            if (month<10) month = "0" + month;
            if (day<10) day = "0" + day;
            sDate = year + "-" + month + "-" + day ;
            window.location="<?=SITE_HTTP_ROOT?>organizer/day/date/"+sDate+"/";
        }
    }
<?
    if(access_enable($STORAGE, 'organizer', 'day')){  
?>  
    YAHOO.yui.quarter.cal3.selectEvent.subscribe(YAHOO.yui.quarter.selectDate);
<?
    };
?>    
}

YAHOO.util.Event.onDOMReady(YAHOO.yui.quarter.init);
</script>
<?
    $ids=calendar::getIdsForEmployee($STORAGE['current_employee']->login);
    $ids[]=0;
    $ids=join(',',$ids);

    $where=array();
    $where[]='"calendar_id" in ('.$ids.')';
    $where[]='"mark"=\'yes\'';
    $where[]='"completed"=\'no\'';
    $date = date('Y-m-01');
    $start_day=strtotime('-1 months',strtotime($date));
    $stop_day=strtotime('+3 months -1 day',$start_day);
    $stop_date=date('Y-m-d',$stop_day);
    $start_date=date('Y-m-d',$start_day);

    $where[]='"start_date"<=\''.$stop_date.'\' AND "stop_date">=\''.$start_date.'\'';

    $query = 'SELECT * FROM '.TABLE_CALENDAR_EVENT.' WHERE '.join(' AND ', $where).' ';
    $db->query($query);
if (count($db->value)>0){
    $events = $db->value;
    $event = new calendar_event();
    for($i=0;$i<count($events);$i++){
      $event->Init();
      $event->LoadItem($events[$i]);
      $start=strtotime($event->start_date);
      $start=($start>$start_day?$start:$start_day);
      $stop=strtotime($event->stop_date);
      $stop=($stop<$stop_day?$stop:$stop_day);
      while($start<=$stop){
        $dates[date('Y',$start)][date('n',$start)][date('j',$start)]=1;
        $start=strtotime('+1 day',$start);
      };
    };
    $selector='#yahoo-com3.yui-skin-eco .yui-calendar.y2000 .m0 .d0 a';
    foreach($dates as $year=>$monthes){
      foreach($monthes as $month=>$days){
        foreach($days as $day=>$val){
          $selector.=','."\n\r".'#yahoo-com3.yui-skin-eco .yui-calendar.y'.$year.' .m'.$month.' .d'.$day.' a';
        }
      }
    }
//    echo $query;
//    print_r($days);
?>
<style type="text/css">
<?=$selector?>{
  background-color:#009FE0;
  color:white;
  border:1px solid #F3F3F3;
}
</style>
<?
}
?>
