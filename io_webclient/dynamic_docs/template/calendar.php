<div id="yahoo-com" class=" yui-skin-sam">
<div id="cal1Container"></div>
</div>

<script language="JavaScript" type="text/javascript">
YAHOO.namespace("yui.calendar");

YAHOO.yui.calendar.init = function() {
	YAHOO.yui.calendar.cal1 = new YAHOO.widget.Calendar(
	                                                        "cal1","cal1Container",
															{
															    <?
															         if (array_key_exists('pagedate', $calendar_params) && isset($calendar_params['pagedate'])) {
															             echo 'pagedate: "'.$calendar_params['pagedate'].'",';
															         }

															         if (array_key_exists('maxdate', $calendar_params) && isset($calendar_params['maxdate'])) {
															             echo 'maxdate: "'.$calendar_params['maxdate'].'",';
															         }
															    ?>
                                                                START_WEEKDAY: 1
                                                            }
                                                        );

    <?
	   if (array_key_exists('selected_date', $calendar_params) && isset($calendar_params['selected_date'])) {
            echo 'YAHOO.yui.calendar.cal1.select("'.$calendar_params['selected_date'].'");';
	   }
	?>

	YAHOO.yui.calendar.cal1.cfg.setProperty("DATE_FIELD_DELIMITER", ".");
	YAHOO.yui.calendar.cal1.cfg.setProperty("MDY_DAY_POSITION", 1);
	YAHOO.yui.calendar.cal1.cfg.setProperty("MDY_MONTH_POSITION", 2);
	YAHOO.yui.calendar.cal1.cfg.setProperty("MDY_YEAR_POSITION", 3);

	YAHOO.yui.calendar.cal1.cfg.setProperty("MD_DAY_POSITION", 1);
	YAHOO.yui.calendar.cal1.cfg.setProperty("MD_MONTH_POSITION", 2);

	YAHOO.yui.calendar.cal1.cfg.setProperty("MONTHS_SHORT",   ["Янв", "Фев", "Март", "Апр", "Май", "Июн", "Июл", "Авг", "Сен", "Окт", "Ноя", "Дек"]);
	YAHOO.yui.calendar.cal1.cfg.setProperty("MONTHS_LONG",    ["Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"]);
	YAHOO.yui.calendar.cal1.cfg.setProperty("WEEKDAYS_1CHAR", ["В", "П", "В", "С", "Ч", "П", "С"]);
	YAHOO.yui.calendar.cal1.cfg.setProperty("WEEKDAYS_SHORT", ["Вс", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб"]);
	YAHOO.yui.calendar.cal1.cfg.setProperty("WEEKDAYS_MEDIUM",["Вос", "Пон", "Вт", "Ср", "Чет", "Пят", "Суб"]);
	YAHOO.yui.calendar.cal1.cfg.setProperty("WEEKDAYS_LONG",  ["Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"]);


    <?
        if ($calendar_params['onSelectHandler']) {
            echo 'YAHOO.yui.calendar.cal1.selectEvent.subscribe('.$calendar_params['onSelectHandler'].', YAHOO.yui.calendar.cal1, true);';
        }
    ?>

	YAHOO.yui.calendar.cal1.render();
}

YAHOO.util.Event.onDOMReady(YAHOO.yui.calendar.init);
</script>
