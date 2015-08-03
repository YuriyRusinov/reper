<?
    if ($save && !$errors) {
?>
<script language="JavaScript">
    
<!--
      window.location="<?=SITE_HTTP_ROOT.$STORAGE['site_path'][count($STORAGE['site_path'])-2]['path']?>";
//-->
</script>
<?
    } else {
    	

        if ($message_journal->insert_time!='') {
        	
        	$date = strtok($message_journal->insert_time, " ");
        	
            $date = format_date('{d}.{m}.{Y}', $date);
            if ($date=='00.00.0000') $date = '';
            
             $start_time = strtok(" ");
        }
        
        if ($message_journal->sent_datetime!='') {
        	
        	$date2 = strtok($message_journal->sent_datetime, " ");
        	
            $date2 = format_date('{d}.{m}.{Y}', $date2);
            if ($date=='00.00.0000') $date = '';
            
             $stop_time = strtok(" ");
        }
        
       
        
        
        if($id == 0) 
        {
        	$date = date("d.m.Y");		
        }
?>

<? include SITE_FILE_ROOT.'template/ocalendar.php'; ?>
<script language="JavaScript" type="text/javascript">
    function setDate(sDate) {
        document.editform["year-field"].value = sDate;
    }
    function selectDate(event){
        var X=event.clientX+16;
        var Y=event.clientY+16;
        if (X+185>parseInt(document.body.clientWidth)) X=X-185;
        if (Y+215>parseInt(document.body.clientHeight)) Y=Y-215;
        YAHOO.yui.oCalendar.open(document.editform["year-field"].value,"",X,Y,setDate);
    }
   
     function setDate2(sDate) {
        document.editform["year-field2"].value = sDate;
    }
    function selectDate2(event){
        var X=event.clientX+16;
        var Y=event.clientY+16;
        if (X+185>parseInt(document.body.clientWidth)) X=X-185;
        if (Y+215>parseInt(document.body.clientHeight)) Y=Y-215;
        YAHOO.yui.oCalendar.open(document.editform["year-field2"].value,"",X,Y,setDate2);
    }
    
    
    function hideCalendar(){
        YAHOO.yui.oCalendar.close();
    }

</script>

<div class="pagename">
    Редактирование сообщения
</div>
<form name="editform" 
      id="editform"
      action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/id/'.$id.'/'; ?>" 
      method="post" 
      onmousedown="hideCalendar();">
<input type="hidden" name="id" value="<?= htmlspecialchars($message_journal->id); ?>" />
<input type="hidden" name="save" value="1" />

<table width="100%" cellpadding="2" cellspacing="2" border="0" class="formz" align="center">
<tr>
    <td width="1%" colspan="3" >
    <table width="100%" class="column" cellpadding="0" cellspacing="0" border="0" >
    <tr>
        
        <td style="text-align:right;">ID:&nbsp;<?= htmlspecialchars($journal->id); ?></td>
    </tr>
    </table>
    </td>
    <td>&nbsp;
    </td>
</tr>
<tr>
    <td valign="top" >
        <table border="0" cellpadding="2" cellspacing="2" width="450" class="column">
        
<tr>
    <td align="right" valign="top">Информационный объект:</td>
    <td width="100%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
            <tr>
                <td width="70%">
                    <select name="io_id" class="wide">
                        <option value="" />                        
                        <?
                            for ($i=0;$i<count($objects);$i++) {
                                ?>
                                    <option value="<?= $objects[$i]->id; ?>"<?=($message_journal->id_io_object==$objects[$i]->id ? ' selected="selected"':'');?> ><?= htmlspecialchars($objects[$i]->name); ?></option>
                                <?
                            }
                        ?>
                    </select>
                </td>
              
                <td><button id="select-employee" onclick="return SelectKioAction();" class="wide"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Выбрать">Выбрать...</button></td>
            	
                </tr>
        </table>
    </td>
</tr> 
      
<tr>
    <td align="right" valign="top">Отправитель сообщения:</td>
    <td width="100%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
            <tr>
                <td width="70%">
                    <select name="sender_id" class="wide" <?=(($message_journal->id_dl_sender==$empl[$i]->id)||($_SESSION['PST']==$empl[$i]->id))? ' disabled':''?> >
                        <option value="" />                        
                        <?
                            for ($i=0;$i<count($empl);$i++) {
                                ?>
                                    <option value="<?= $empl[$i]->id; ?>"<?=((($message_journal->id_dl_sender==$empl[$i]->id)||($_SESSION['PST']==$empl[$i]->id)) ? ' selected="selected"':'');?> ><?= htmlspecialchars($empl[$i]->name); ?></option>
                                <?
                            }
                        ?>
                    </select>
                </td>
              <?
                //<td><button id="select-employee" onclick="return SelectAction();" class="wide"><img src="<?= SITE_URI_ROOT; 
                //img/iconEdit.gif" title="Выбрать">Выбрать...</button></td>
            	?>
                </tr>
        </table>
    </td>
</tr>
        
           <tr>
    <td align="right" valign="top">Получатель сообщения:</td>
    <td width="100%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
            <tr>
                <td width="70%">
                    <select name="receiver_id" class="wide">
                        <option value="" />                        
                        <?
                            for ($i=0;$i<count($empl);$i++) {
                                ?>
                                    <option value="<?= $empl[$i]->id; ?>"<?=($message_journal->id_dl_receiver==$empl[$i]->id ? ' selected="selected"':'');?> ><?= htmlspecialchars($empl[$i]->name); ?></option>
                                <?
                            }
                        ?>
                    </select>
                </td>
                <td><button id="select-employee" onclick="return SelectExecAction();" class="wide"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Выбрать">Выбрать...</button></td>
            </tr>
        </table>
    </td>
</tr>
		
		
	  
       
        </table>
    </td>
    <td>
        <table border="0" width="50" class="column">
        </table>
    </td>
    <td valign="top" >
        <table border="0" cellpadding="2" cellspacing="2" width="450" class="column">
       
        <tr>
            <td align="right" width="100"> Внесено в БД(дата дд.мм.гггг):</td>
            <td>
                <input id="year-field" type="text" name="date" value="<?= htmlspecialchars($date); ?>"  class="date"/>
                <button onclick="selectDate(event);return false;" class="minibutton"><img src="<?= SITE_URI_ROOT; ?>img/iconCalendar.gif" height="16" width="16" border="0" id="calbtn3"/></button>
            </td>
        </tr>  
        <tr>
    	<td align="right">Время:</td>
    	
    	<? if($id === 0) $start_time = "00:00";?>
    	<td><input type="text" name="start_time" value="<?= htmlspecialchars($start_time); ?>" class="date" /></td>
		</tr>
		 <tr>
            <td align="right" width="100">Отправлено (дата дд.мм.гггг):</td>
            <td>
                <input id="year-field2" type="text" name="date2" value="<?= htmlspecialchars($date); ?>"  class="date"/>
                <button onclick="selectDate2(event);return false;" class="minibutton"><img src="<?= SITE_URI_ROOT; ?>img/iconCalendar.gif" height="16" width="16" border="0" id="calbtn3"/></button>
            </td>
        </tr> 
		
		<tr>
    	<td align="right">Время:</td>
    	<? if($id ===0) $stop_time = "00:00";?>
    	<td><input type="text" name="stop_time" value="<?= htmlspecialchars($stop_time); ?>" class="date" /></td>
		</tr>
		<tr>
		<td align="right">Текст сообщения:</td>
              <td>  <textarea name="description" class="wide" rows="4"><?= htmlspecialchars($message_journal->message_body); ?></textarea>
            </td>
		</tr>
       
      
        </table>
    </td>
  
    <td>&nbsp;
    </td>
</tr>

<tr>
    <td colspan="3" class="toolbar">

        <button type="submit" class="button" >Сохранить &raquo;</button>  
  
    </td> 
    <td>&nbsp;
    </td>
</tr>
</table>



</form>




<script language="JavaScript" type="text/javascript">
<!--

function SetKioAction(value, text)
{
    var list = document.editform.elements['io_id'];

    selectOption(list, value);
}

function SelectKioAction()
{
    var url = '<?= SITE_HTTP_ROOT; ?>message/select-object/callback/SetKioAction';

    var w = 500;
    var h = 400;
    var param = '';
    OpenPopupWindow(url, w, h, param);

    return false;
}


function SetAction(value, text)
{
    var list = document.editform.elements['sender_id'];

    selectOption(list, value);
}

function SelectAction()
{
    var url = '<?= SITE_HTTP_ROOT; ?>message/select-dls/callback/SetAction';

    var w = 550;
    var h = 200;
    var param = '';
    OpenPopupWindow(url, w, h, param);

    return false;
}

function SetExecAction(value, text)
{
    var list = document.editform.elements['receiver_id'];

    selectOption(list, value);
}

function SelectExecAction()
{
    var url = '<?= SITE_HTTP_ROOT; ?>message/select-dls/callback/SetExecAction';

    var w = 550;
    var h = 200;
    var param = '';
    OpenPopupWindow(url, w, h, param);

    return false;
}






//-->
</script>


        <?
    }
?>
