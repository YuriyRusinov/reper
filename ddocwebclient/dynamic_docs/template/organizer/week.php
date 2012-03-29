<?
  $months_long=array("Января", "Февраля", "Марта", "Апреля", "Мая", "Июня", "Июля", "Августа", "Сентября", "Октября", "Ноября", "Декабря");
  $weekdays_long=array("воскресенье", "понедельник", "вторник", "среда", "четверг", "пятница", "суббота");
  $events_exist=(count($week_events)>0 || count($time_events)>0);
?>
<script type="text/javascript" language="JavaScript">
    function addEvent(){
        var url="<?= SITE_HTTP_ROOT.$STORAGE['module']; ?>/edit/start_date/<?=$start_date?>/stop_date/<?=$stop_date?>";
        window.location=url;
        return false; 
    }
    function showDate(date){
        var url="<?= SITE_HTTP_ROOT.$STORAGE['module']; ?>/day/date/"+date+"/";
        window.location=url;
        return false; 
    }
    function showWeek(date){
        var url="<?= SITE_HTTP_ROOT.$STORAGE['module']; ?>/week/date/"+date+"/";
        window.location=url;
        return false; 
    }
    function editEvent(id){
        var url="<?= SITE_HTTP_ROOT.$STORAGE['module']; ?>/edit/id/"+id+"/";
        window.location=url;
        return false;         
    }
    function deleteEvent(id){
        var url="<?= SITE_HTTP_ROOT.$STORAGE['module']; ?>/delete/id/"+id+"/";
        url+="callback/week/date/<?=$date?>/";
        window.location=url;
        return false;         
    }
</script>
    <div class="">
        <button onclick="return addEvent();">Запланировать событие</button>
    </div>
<?
if($events_exist){
?>
    <div class="pagename">
        Cобытия запланированые c <?=format_date('{d}', $start_date); ?> 
                               по <?=format_date('{d} '.$months_long[date('n',$stop_day)-1].' {Y}', $stop_date); ?>
    </div>
<?
}else{
?>
    <div class="pagename">
       На период c <?=format_date('{d}', $start_date); ?> 
                 по <?=format_date('{d} '.$months_long[date('n',$stop_day)-1].' {Y}', $stop_date); ?> 
       событий не запланировано
    </div>
<?    
}
?>
    <div class="toolbar half floatLeft">
        <button onclick="return showWeek('<?=date('Y-m-d',strtotime('-1 week',$stop_day))?>');">на предыдущую</button>
        <button onclick="return showWeek('<?=date('Y-m-d',strtotime('+1 week',$stop_day))?>');">на следующею</button>
    </div>
    <div class="toolbar half floatRight right">
        <button onclick="return showDate('<?=date('Y-m-d')?>');">на сегодня</button>    
        <button onclick="return showWeek('<?=date('Y-m-d')?>');" <?=(date('Y-m-d',strtotime('this Sunday'))==$stop_date?'disabled="disabled"':'')?>>на текущую неделю</button>
    </div>

<?
if($events_exist){
?>
    <table cellspacing="1" border="0" class="list" width="100%">
    <tr>
        <th width="2%">№</th>
        <th width="18%">Срок</th>
        <th width="78%">Тема</th>
        <th width="2%">&nbsp;</th>
    </tr>
<?
    $event=new calendar_event();
    for($i=0;$i<count($week_events);$i++){
        $event->Init();
        $event->LoadItem($week_events[$i]);
        if(!trim($event->what)){
            $event->what='Без темы';
        };
?>
    <tr ondblclick="return editEvent(<?=$event->id_event?>);" class="active" title="Двойной щелчёк для редактирования">
        <td><?=$i+1?>.</td>
        <td nowrap><?=format_date('{d}/{m}/{Y}',$event->start_date);?> - <?=format_date('{d}/{m}/{Y}',$event->stop_date);?></td>
        <td <?=($event->completed=='yes'?'class="strikeout"':'')?>>
<?
        if($event->completed=='no' && $event->notice=='yes'){
?>
          <img src="<?= SITE_URI_ROOT; ?>img/iconNotice.gif" title="Показывать уведомление" />
<?
        }
        if($event->completed=='no' && $event->mark=='yes'){
?>
          <img src="<?= SITE_URI_ROOT; ?>img/iconWarning.gif" title="Отметка в календаре" />
<?
        }
?>
          <?=htmlspecialchars($event->what);?>
        </td>
        <td nowrap>
<?        
        if($event->CheckActionEnable($STORAGE, 'edit')){
?>
            <button onclick="return editEvent(<?= $event->id_event; ?>)" class="minibutton" title="Редактировать событие"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" /></button>
<?
        }
?>
<?        
        if($event->CheckActionEnable($STORAGE, 'delete')){
?>
            <button onclick="return deleteEvent(<?= $event->id_event; ?>)" class="minibutton" title="Удалить событие"><img src="<?= SITE_URI_ROOT; ?>img/iconDelete.gif" /></button>
<?
        }
?>
        </td>
    </tr>
<?    
    }
    $n=$i+1;
    
    if(count($time_events)>0){
        $prev_date=$start_date;
        for($i=0;$i<count($time_events);$i++){
            $event->Init();
            $event->LoadItem($time_events[$i]);                       
            if(!trim($event->what)){
                $event->what='Без темы';
            };
            while($prev_date<=$event->start_date){
            $prev_day=strtotime($prev_date);
?>
    <tr ondblclick="return showDate('<?=$prev_date?>');" class="active">
        <th colspan="4"> <a href="<?= SITE_HTTP_ROOT.$STORAGE['module']; ?>/day/date/<?=$prev_date?>"><?=date('j ',$prev_day).$months_long[date('n',$prev_day)-1]; ?></a>, <?=$weekdays_long[date('w',$prev_day)];?></th>
    </tr>
<?        
            $prev_date=date('Y-m-d',strtotime('+1 day',$prev_day));
                
            };
?>
    <tr ondblclick="return editEvent(<?=$event->id_event?>);" class="active" title="Двойной щелчёк для редактирования">
        <td><?=$i+$n?>.</td>        
        <td  nowrap>
<?
        if($event->start_time==''){
?>            
            весь день
<?
        }else{
?>
            <?=format_time('{H}:{i}',$event->start_time);?> - <?=format_time('{H}:{i}',$event->stop_time);?>
<?
        }    
?>        
        </td>
        <td <?=($event->completed=='yes'?'class="strikeout"':'')?>>
<?
        if($event->completed=='no' && $event->notice=='yes'){
?>
          <img src="<?= SITE_URI_ROOT; ?>img/iconNotice.gif" title="Показывать уведомление" />
<?
        }
        if($event->completed=='no' && $event->mark=='yes'){
?>
          <img src="<?= SITE_URI_ROOT; ?>img/iconWarning.gif" title="Отметка в календаре" />
<?
        }
?>
          <?=htmlspecialchars($event->what);?>
        </td>
        <td  nowrap>
<?        
        if($event->CheckActionEnable($STORAGE, 'edit')){
?>
            <button onclick="return editEvent(<?= $event->id_event; ?>)" class="minibutton" title="Редактировать событие"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" /></button>
<?
        }
?>
<?        
        if($event->CheckActionEnable($STORAGE,'delete')){
?>
            <button onclick="return deleteEvent(<?= $event->id_event; ?>)" class="minibutton" title="Удалить событие"><img src="<?= SITE_URI_ROOT; ?>img/iconDelete.gif" /></button>
<?
        }
?>
        </td>
    </tr>
<?    
        }
        while($prev_date<=$stop_date){
            $prev_day=strtotime($prev_date);
?>
    <tr ondblclick="return showDate('<?=$prev_date?>');" class="active">
        <th colspan="4"> <a href="<?= SITE_HTTP_ROOT.$STORAGE['module']; ?>/day/date/<?=$prev_date?>"><?=date('j ',$prev_day).$months_long[date('n',$prev_day)-1]; ?></a>, <?=$weekdays_long[date('w',$prev_day)];?></th>
    </tr>
<?        
            $prev_date=date('Y-m-d',strtotime('+1 day',$prev_day));
        };
        
    }
?>    
    </table>
<?
}
?>