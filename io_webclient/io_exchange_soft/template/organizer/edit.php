<?
if ($save && !$errors) {
?>
<script language="JavaScript">
    
<!--
      window.location="<?=SITE_HTTP_ROOT?>organizer/day/date/<?=parse_date('{Y}-{m}-{d}',$event->start_date,true);?>/";
//-->
</script>
<?
} else {
    $start_date = '';
    $stop_date = '';
    $start_time = '10:00';
    $stop_time = '11:00';
    if ($event->start_date!='') {
        $start_date = format_date('{d}.{m}.{Y}', $event->start_date);
        if ($start_date=='00.00.0000') $start_date = '';
    }
    if ($event->stop_date!='') {
        $stop_date = format_date('{d}.{m}.{Y}', $event->stop_date);
        if ($stop_date=='00.00.0000') $stop_date = '';
    }
    if ($event->start_time!=''){
        $start_time = format_time('{H}:{i}',$event->start_time);
    }
    if ($event->stop_time!=''){
        $stop_time = format_time('{H}:{i}',$event->stop_time);
    }
?>
    
<? include SITE_FILE_ROOT.'template/ocalendar.php'; ?>
<script language="JavaScript" type="text/javascript">
    function setDate(sDate) {
        document.editform["start_date"].value = sDate;
    }
    function selectDate(event){
        var pos=$("start_date").cumulativeOffset();
        var X=pos[0];
        var Y=pos[1]+$("start_date").offsetHeight;
        YAHOO.yui.oCalendar.open(document.editform["start_date"].value,"",X,Y,setDate);
    }
    function setDate2(sDate) {
        document.editform["stop_date"].value = sDate;
    }
    function selectDate2(event){
        var pos=$("stop_date").cumulativeOffset();
        var X=pos[0];
        var Y=pos[1]+$("stop_date").offsetHeight;
        YAHOO.yui.oCalendar.open(document.editform["stop_date"].value,"",X,Y,setDate2);
    }
    function hideCalendar(){
        YAHOO.yui.oCalendar.close();
    }
    function showTimes(){
        if(document.editform["wholeday"].checked){
            $("start_time").hide();
            $("stop_time").hide();
        }
        else
        {
            $("start_time").show();
            $("stop_time").show();            
        }
    }
</script>
<div class="pagename">
    Карта события
</div>
<form name="editform"
      action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" 
      method="post" 
      onmousedown="hideCalendar()"
      >
<input type="hidden" name="id" value="<?=$event->id_event;?>" />
<input type="hidden" name="save" value="1" />
<table width="100%" cellpadding="3" cellspacing="0" border="0" class="formz" align="center">
<tr>
    <td width="1%" >
    <table width="600" class="column" cellpadding="0" cellspacing="0" border="0" >
    <tr>
        <td nowrap width="100">Событие:&nbsp;</td>
        <td ><input type="text" name="what" value="<?=  htmlspecialchars(trim($event->what)); ?>" class="wide" /></td>
    </tr>
    <tr>
        <td nowrap >Дата:&nbsp;</td>
        <td nowrap >
        <input type="text" id="start_date" name="start_date" value="<?=  htmlspecialchars($start_date); ?>" class="date" onfocus="selectDate(event)" onclick="selectDate(event)"/>
        <input type="text" id="start_time" name="start_time" value="<?=  htmlspecialchars($start_time); ?>" class="time" style="<?=($wholeday?'display:none':'');?>" >
        -
        <input type="text" id="stop_time" name="stop_time" value="<?=  htmlspecialchars($stop_time); ?>" class="time" style="<?=($wholeday?'display:none':'');?>" >
        <input type="text" id="stop_date" name="stop_date" value="<?=  htmlspecialchars($stop_date); ?>" class="date" onfocus="selectDate2(event)" onclick="selectDate2(event)"/>
        <input type="checkbox" id="wholeday" name="wholeday" <?=($wholeday?'checked="checked"':'');?> value="1" onclick="return showTimes();" /> <label for="wholeday">Весь день</label>
        </td>
    </tr>
    <tr>
        <td nowrap >Место:&nbsp;</td>
        <td ><input type="text" name="where" value="<?=  htmlspecialchars(trim($event->where)); ?>" class="wide" /></td>
    </tr>
    <tr>
        <td nowrap class="top" >Описание:&nbsp;</td>
        <td ><textarea name="description" class="wide" rows="7"><?=  htmlspecialchars(trim($event->description)); ?></textarea></td>
    </tr>
    <tr>
        <td nowrap class="top" >&nbsp;</td>
        <td ><input type="checkbox" id="notice" name="notice" <?=($event->notice=='yes'?'checked="checked"':'');?> value="yes" /> <label for="notice">Показывать уведомление</label></td>
    </tr>
    <tr>
        <td nowrap class="top" >&nbsp;</td>
        <td ><input type="checkbox" id="mark" name="mark" <?=($event->mark=='yes'?'checked="checked"':'');?> value="yes" /> <label for="mark">Отметить в календаре</label></td>
    </tr>
    <tr>
        <td nowrap class="top" >&nbsp;</td>
        <td ><input type="checkbox" id="completed" name="completed" <?=($event->completed=='yes'?'checked="checked"':'');?> value="yes" /> <label for="completed">Отметить как завершённое</label></td>
    </tr>
    <tr class="toolbar">
        <td colspan="2"><button type="submit" class="button">Сохранить &raquo;</button></td>    
    </tr>
    </table>
    </td>
    <td>&nbsp;
    </td>
</tr>
</table>
</form>

<?
}
?>