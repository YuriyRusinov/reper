<?
  $months_long=array("Января", "Февраля", "Марта", "Апреля", "Мая", "Июня", "Июля", "Августа", "Сентября", "Октября", "Ноября", "Декабря");
  $weekdays_long=array("воскресенье", "понедельник", "вторник", "среда", "четверг", "пятница", "суббота");
  $day=strtotime($date);
  $events_exist=(count($day_events)>0 || count($time_events)>0);
?>
<script type="text/javascript" language="JavaScript">
    function addEvent(){
        var url="<?= SITE_HTTP_ROOT.$STORAGE['module']; ?>/edit/start_date/<?=$date?>/";
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
        url+="callback/day/date/<?=$date?>/";
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
        Cобытия запланированые на <?=format_date('{d} '.$months_long[date('n',$day)-1].' {Y}', $date); ?>, <?=$weekdays_long[date("w",$day)];?>
    </div>
<?
}else{
?>
    <div class="pagename">
       На <?=format_date('{d} '.$months_long[date('n',$day)-1].' {Y}', $date); ?> (<?=$weekdays_long[date("w",$day)];?>) событий не запланировано
    </div>
<?    
}
?>
    <div class="toolbar  half floatLeft">
        <button onclick="return showDate('<?=date('Y-m-d',strtotime('-1 day',$day))?>');">на предыдущий</button>
        <button onclick="return showDate('<?=date('Y-m-d',strtotime('+1 day',$day))?>');">на следующий</button>
    </div>
    <div class="toolbar half floatRight right">
        <button onclick="return showDate('<?=date('Y-m-d')?>');" <?=(date('Y-m-d')==$date?'disabled="disabled"':'')?>>на сегодня</button>
        <button onclick="return showWeek('<?=date('Y-m-d')?>');">на текущую неделю</button>
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
    for($i=0;$i<count($day_events);$i++){
        $event->Init();
        $event->LoadItem($day_events[$i]);
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
?>
    <tr>
        <th colspan="4">На день</th>
    </tr>
<?        
    }
    for($i=0;$i<count($time_events);$i++){
        $event->Init();
        $event->LoadItem($time_events[$i]);
        if(!trim($event->what)){
            $event->what='Без темы';
        };
?>
    <tr ondblclick="return editEvent(<?=$event->id_event?>);" class="active" title="Двойной щелчёк для редактирования">
        <td><?=$i+$n?>.</td>
        <td  nowrap><?=format_time('{H}:{i}',$event->start_time);?> - <?=format_time('{H}:{i}',$event->stop_time);?></td>
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
?>
    </table>
<?
}
?>