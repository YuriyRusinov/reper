<?
    if ($save && !$errors) {
?>
<script language="JavaScript">
    
<!--

     window.location = "<?=SITE_HTTP_ROOT.$STORAGE['site_path'][count($STORAGE['site_path'])-2]['path']?>";

 //-->
</script>
<?
    } else {
    	

        if ($io_object->insert_time!='') {
        	
        	$date = strtok($io_object->insert_time, " ");
        	
            $date = format_date('{d}.{m}.{Y}', $date);
            if ($date=='00.00.0000') $date = '';
            
             $start_time = strtok(" ");
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
   
   
    function hideCalendar(){
        YAHOO.yui.oCalendar.close();
    }

</script>

<div class="pagename">
    Редактирование информационного объекта
</div>




<form name="editform" 
      id="editform"
      action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/id/'.$id.'/'; ?>" 
      method="post" 
      onmousedown="hideCalendar();">
<input type="hidden" name="id" value="<?= htmlspecialchars($io_object->id); ?>" />
<input type="hidden" name="is_system" value="<?= htmlspecialchars($io_object->is_system); ?>" />

<input type="hidden" name="jr_id" value="<?= htmlspecialchars($jr_id); ?>" />
<input type="hidden" name="table_name" value="<?= htmlspecialchars($io_object->table_name); ?>" />
<input type="hidden" name="save" value="1" />

<table width="100%" cellpadding="2" cellspacing="2" border="0" class="formz" align="center">
<tr>
    <td width="1%" colspan="3" >
    <table width="100%" class="column" cellpadding="0" cellspacing="0" border="0" >
    <tr>
        
        <td style="text-align:right;">ID:&nbsp;<?= htmlspecialchars($io_object->id); ?></td>
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
		<td align="right">Название информационного объекта</td>
              <td>  <input type="text" name="name" class="wide" rows="4" value="<?= htmlspecialchars($io_object->name); ?>">
            </td>
		</tr>
        
        
        
        <tr>
    <td align="right" valign="top">Категория И.О:</td>
    <td width="100%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
            <tr>
                <td width="70%">
                <?
                
               for($j=0;$j<count($cats);$j++)
               {
               		if($io_object->id_io_category == $cats[$j]->id)
               		{
               			$kio = $cats[$j]->name;
               			break;
               		}
               }
                ?>
                    <input type="text" name="kio" value="<?= htmlspecialchars($kio); ?>" class="wide" <?= $id > 0 ? 'disabled':''?> />
                    <input type="hidden" name="cat_id" value="<?= $io_object->id_io_category; ?>" />
                
                    </td>
                    <? if ($id === 0 && $io_object->id_io_category <= 0)
                    {
                    	?>
                <td><button id="select-department" onclick="return SelectKioAction();" class="wide"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Выбрать Категорию" />Выбрать...</button></td>
            <?     } 
            
                        ?>
                </tr>
        </table>
    </td>
</tr>  
       
   
        
           <tr>
    <td align="right" valign="top">Автор:</td>
    <td width="100%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
            <tr>
                <td width="70%">
                    <select name="aut_id" class="wide" <?= $id > 0 ? 'disabled':'' ?> >
                        <option value="" />                        
                        <?
                            for ($i=0;$i<count($empl);$i++) {
                                ?>
                                    <option value="<?= $empl[$i]->id; ?>"<?=($io_object->author==$empl[$i]->id ? ' selected="selected"':'');?> ><?= htmlspecialchars($empl[$i]->fio); ?></option>
                                <?
                            }
                        ?>
                    </select>
                </td>
                
            </tr>
        </table>
    </td>
</tr>
		
		
           <tr>
    <td align="right" valign="top">Состояние:</td>
    <td width="100%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
            <tr>
                <td width="70%">
                    <select name="state_id" class="wide" <?= $id===0 ? 'disabled':''?> >
                        <option value="" />                        
                        <?
                            for ($i=0;$i<count($states);$i++) {
                                ?>
                                    <option value="<?= $states[$i]->id; ?>"<?=(($io_object->id_io_state==$states[$i]->id ) || ($id == 0 && $i ==0) ? ' selected="selected"':'');?> ><?= htmlspecialchars($states[$i]->name); ?></option>
                                <?
                            }
                        ?>
                    </select>
                </td>
                <td><button id="select-state" <?= $id===0 ? 'disabled':'' ?> onclick="return SelectJRAction();" class="wide"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Выбрать">Выбрать...</button></td>
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
		<td align="right">Описание:</td>
              <td>  <textarea name="description" class="wide" rows="4"><?= htmlspecialchars($io_object->description); ?></textarea>
            </td>
		</tr>
       
		
		<tr>
		<td align="right">Дополнительная информация об ИО:</td>
              <td>  <textarea name="information" class="wide" rows="4"><?= htmlspecialchars($io_object->information); ?></textarea>
            </td>
		</tr>
		
		
		
		
		<tr>
		<td align="right">Название таблицы:</td>
              <td>  <input  type="text" name="table_nam" class="wide" rows="4" value="<?= htmlspecialchars($io_object->table_name); ?>" disabled/>
            </td>
		</tr>
		
		 <tr>
            <td align="right" width="100"> Внесена в БД(дата дд.мм.гггг):</td>
            <td>
                <input  id="year-field" type="text" name="date" value="<?= htmlspecialchars($date); ?>"  class="date"/>
                <button <?=$io_object->id > 0 ?'disabled':''    ?> onclick="selectDate(event);return false;" class="minibutton"><img src="<?= SITE_URI_ROOT; ?>img/iconCalendar.gif" height="16" width="16" border="0" id="calbtn3"/></button>
            </td>
        </tr>  
      
         <tr>
    <td align="right" valign="top">Гриф секретности:</td>
    <td width="100%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
            <tr>
                <td width="70%">                
                    <select name="mac_id" class="wide"  <?= $id > 0 ? 'disabled':'' ?> >
                        <option value="" />                        
                        
                    <option value="1"<?=($io_object->id_maclabel==1 ? ' selected="selected"':'');?> >Несекретно </option>
                    <option value="2"<?=($io_object->id_maclabel==2? ' selected="selected"':'');?> >ДСП </option>
                    <option value="3"<?=($io_object->id_maclabel==3 ? ' selected="selected"':'');?> >Секретно </option>
                    <option value="4"<?=($io_object->id_maclabel==4 ? ' selected="selected"':'');?> >Совершено секретно </option>
                    <option value="5"<?=($io_object->id_maclabel==5 ? ' selected="selected"':'');?> >Особой важности </option>
                   
                   
                           
                    </select>
                
                
                
                
                    </td>
             
            </tr>
        </table>
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

<td colspan="3">


<?

if($io_object->is_system === FALSE || $io_object->is_system == 'f'){
?>


<button id="edit-atribute-<?= $id; ?>" onclick="AtrAction(<?= $id; ?>)" title="Редактировать атрибуты"><img src="<?= SITE_URI_ROOT; ?>img/iconNotice.gif" /></button>

<?
}
?>




<?

if($io_object->table_name){
?>


<button id="edit-tables-<?= $id; ?>" onclick="TableAction(<?= $id; ?>)" title="Редактировать вложения"><img src="<?= SITE_URI_ROOT; ?>img/figa.gif" /></button>

<?
}
?>



<?

if($io_object->id > 0)
{

?>

<button id="edit-journal-<?= $id; ?>" onclick="EditAction(<?= $id; ?>)" title="Редактировать приложенные рубрики"><img src="<?= SITE_URI_ROOT; ?>img/iconDetail.gif" /></button>
                	
<button id="files-journal-<?= $id; ?>" onclick="FileAction(<?= $id; ?>)" title="Редактировать приложенные файлы"><img src="<?= SITE_URI_ROOT; ?>img/iconNew.gif" /></button>

<?

}
?>
				                			
</td> 


<script language="JavaScript" type="text/javascript">
<!--

function SetKioAction(value, text)
{
    document.editform.elements['kio'].value = text;
    document.editform.elements['cat_id'].value = value;
}

function SelectKioAction()
{
    var url = '<?= SITE_HTTP_ROOT; ?>journal/select-kio/callback/SetKioAction';

    var w = 500;
    var h = 400;
    var param = '';
    OpenPopupWindow(url, w, h, param);

    return false;
}


function SetAction(value, text)
{
    var list = document.editform.elements['ika_id'];

    selectOption(list, value);
    
    
    
}

function SelectAction()
{
    var url = '<?= SITE_HTTP_ROOT; ?>journal/select-dls/callback/SetAction';

    var w = 550;
    var h = 200;
    var param = '';
    OpenPopupWindow(url, w, h, param);

    return false;
}

function SetExecAction(value, text)
{
    var list = document.editform.elements['exec_id'];

    selectOption(list, value);
}

function SelectExecAction()
{
    var url = '<?= SITE_HTTP_ROOT; ?>journal/select-dls/callback/SetExecAction';

    var w = 550;
    var h = 200;
    var param = '';
    OpenPopupWindow(url, w, h, param);

    return false;
}

function SetFromAction(value, text)
{
    var list = document.editform.elements['from_id'];

    selectOption(list, value);
}

function SelectFromAction()
{
    var url = '<?= SITE_HTTP_ROOT; ?>journal/select-dls/callback/SetFromAction';

    var w = 550;
    var h = 200;
    var param = '';
    OpenPopupWindow(url, w, h, param);

    return false;
}

function SetJRAction(value, text)
{
    var list = document.editform.elements['jr_state'];

    selectOption(list, value);
}

function SelectJRAction()
{
    var url = '<?= SITE_HTTP_ROOT; ?>journal/select-jrstate/callback/SetJRAction';

    var w = 550;
    var h = 200;
    var param = '';
    OpenPopupWindow(url, w, h, param);

    return false;
}



function EditAction(id)
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'rubrics')) {
            ?>
                var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/rubrics/id/' + id + '/';
                document.editform.elements['save'].value = 0;
                window.location=url;
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}

function AtrAction(id)
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'attrs')) {
            ?>
                var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/attrs/id/' + id + '/';
                document.editform.elements['save'].value = 0;
                window.location=url;
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}



function TableAction(id)
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'tables')) {
            ?>
                var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/tables/id/' + id + '/';
                document.editform.elements['save'].value = 0;
                window.location=url;
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}



function FileAction(id)
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'files')) {
            ?>
                var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/files/id/' + id + '/';
                window.location=url;
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}


//-->
</script>


        <?
    }
?>
