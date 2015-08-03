<div id="yahoo-com" class=" yui-skin-sam">     
    <div id="caloverlay" style="position: absolute;"></div>
</div>
<script language="JavaScript" type="text/javascript">

    YAHOO.namespace("yui.oCalendar");

    YAHOO.yui.oCalendar.init = function() {
        // Create an Overlay instance to house the Calendar instance
        var navConfig = { 
    	      strings : { 
    	          month: "Месяц:", 
    	          year: "Год:", 
    	          submit: "OK", 
    	          cancel: "Отмена", 
    	          invalidYear: "Введите правильно год" 
              }, 
    	      monthFormat: YAHOO.widget.Calendar.LONG, 
    	      initialFocus: "year" 
    	}; 

        YAHOO.yui.oCalendar.caloverlay = new YAHOO.widget.Overlay("caloverlay",{ visible: false});
        
        YAHOO.yui.oCalendar.caloverlay.render();
        YAHOO.yui.oCalendar.caloverlay.showAt = function (x,y){
            YAHOO.yui.oCalendar.caloverlay.cfg.setProperty("x",x);
            YAHOO.yui.oCalendar.caloverlay.cfg.setProperty("y",y);
            YAHOO.yui.oCalendar.caloverlay.cfg.setProperty("zIndex",20);             
            YAHOO.yui.oCalendar.caloverlay.show();
        }

        /*
           Create a Calendar instance and render it into the body 
           element of the Overlay.
        */        
        
        YAHOO.yui.oCalendar.cal=new YAHOO.widget.Calendar("buttoncalendar", "caloverlay", {navigator:navConfig});

        YAHOO.yui.oCalendar.cal.cfg.setProperty("DATE_FIELD_DELIMITER", ".");
        YAHOO.yui.oCalendar.cal.cfg.setProperty("MDY_DAY_POSITION", 1);
    	YAHOO.yui.oCalendar.cal.cfg.setProperty("MDY_MONTH_POSITION", 2);
    	YAHOO.yui.oCalendar.cal.cfg.setProperty("MDY_YEAR_POSITION", 3);

    	YAHOO.yui.oCalendar.cal.cfg.setProperty("MD_DAY_POSITION", 1);
    	YAHOO.yui.oCalendar.cal.cfg.setProperty("MD_MONTH_POSITION", 2);

    	YAHOO.yui.oCalendar.cal.cfg.setProperty("MONTHS_SHORT",   ["Янв", "Фев", "Март", "Апр", "Май", "Июн", "Июл", "Авг", "Сен", "Окт", "Ноя", "Дек"]);
    	YAHOO.yui.oCalendar.cal.cfg.setProperty("MONTHS_LONG",    ["Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"]);
    	YAHOO.yui.oCalendar.cal.cfg.setProperty("WEEKDAYS_1CHAR", ["В", "П", "В", "С", "Ч", "П", "С"]);
    	YAHOO.yui.oCalendar.cal.cfg.setProperty("WEEKDAYS_SHORT", ["Вс", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб"]);
    	YAHOO.yui.oCalendar.cal.cfg.setProperty("WEEKDAYS_MEDIUM",["Вос", "Пон", "Вт", "Ср", "Чет", "Пят", "Суб"]);
    	YAHOO.yui.oCalendar.cal.cfg.setProperty("WEEKDAYS_LONG",  ["Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"]);
        YAHOO.yui.oCalendar.cal.cfg.setProperty("START_WEEKDAY", 1);
            
        
        YAHOO.yui.oCalendar.close = function(){
            YAHOO.yui.oCalendar.cal.callback=null;                
            YAHOO.yui.oCalendar.caloverlay.hide();        
        }
        
        YAHOO.yui.oCalendar.open = function(selectedDate,maxDate,X,Y,callback){
            var pageDate=selectedDate.substr(3);
            YAHOO.yui.oCalendar.cal.cfg.setProperty("pagedate", pageDate, false);            
            YAHOO.yui.oCalendar.cal.cfg.setProperty("selected", selectedDate, false);            
            YAHOO.yui.oCalendar.cal.cfg.setProperty("maxdate", maxDate, false);            
            YAHOO.yui.oCalendar.cal.render();
            YAHOO.yui.oCalendar.cal.callback=callback;
            YAHOO.yui.oCalendar.caloverlay.showAt(X,Y);  
        
        }
        
        YAHOO.yui.oCalendar.cal.selectDate = function (p_sType, p_aArgs) {
            var aDate;
            var sDate;

            if (p_aArgs) {
                aDate = p_aArgs[0][0];
                var year = aDate[0], month = aDate[1], day = aDate[2];
                if (month<10) month = '0' + month;
                if (day<10) day = '0' + day;
                sDate = day + '.' + month + '.' + year;
                if (YAHOO.yui.oCalendar.cal.callback) YAHOO.yui.oCalendar.cal.callback(sDate);
            }
            YAHOO.yui.oCalendar.close();
        }
        /*
                Subscribe to the Calendar instance's "select" event to 
                update the month, day, year form fields when the user
                selects a date.
        */
        YAHOO.yui.oCalendar.cal.selectEvent.subscribe(YAHOO.yui.oCalendar.cal.selectDate);
//        YAHOO.util.Event.addListener(YAHOO.yui.oCalendar.caloverlay.id, "mouseout", YAHOO.yui.oCalendar.close); 
    }

    YAHOO.util.Event.onDOMReady(YAHOO.yui.oCalendar.init);
  
</script>