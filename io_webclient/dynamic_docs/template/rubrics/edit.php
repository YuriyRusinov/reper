<?

function get_io_id($id)
{
	if($id > 0 )
	{
		$rubrics = new collection();
    	$rubrics->Load(TABLE_RUBRICATOR, false, $where);
		$rubrics = $rubrics->_collection;
	 
	 for($i=0;$i<count($rubrics);$i++)
	 {
	 		if($id == $rubrics[$i]->id)
	 		{
	 		 
	 			if($rubrics[$i]->id_io_object > 0)
	 			{
	 				return $rubrics[$i]->id_io_object;
	 			}
	 			else 
	 			{
	 				if($rubrics[$i]->id_parent > 0)
	 				{
	 					return get_io_id($rubrics[$i]->id_parent);
	 				}
	 				else 
	 				{
	 					return 0;
	 				}
	 			}
	 				
	 		}
	 }
	 return 0;
	}
	else 
	{
		return 0;
	}
	
}


    if ($save && !$errors) {
?>
<script language="JavaScript">
    
<!--
    <?
    if($rubricator->id > 0)
    {
    
    	$io_id = get_io_id($rubricator->id);
	 
	 	
    	if($io_id > 0)
    	{
    	?>
	
    		url  = "<?=SITE_HTTP_ROOT;?>" + "slovar/rubrics/id/" + "<?=$io_id;?>/";
    
    
    		window.location = url;	
    	<?
    	}
    	else 
    	{
    		?>
    		 url = "<?=SITE_HTTP_ROOT.$STORAGE['site_path'][count($STORAGE['site_path'])-2]['path']?>";
	window.location = url;
    		
    		<?
    	}
    }
    else 
    {
    ?>
    
     url = "<?=SITE_HTTP_ROOT.$STORAGE['site_path'][count($STORAGE['site_path'])-2]['path']?>";
	window.location = url;
     <?
    	}
    ?>
     
     //-->
</script>
<?
    } else {
    	      
?>

<div class="pagename">
    Редактирование рубрики
</div>
<form name="editform" 
      id="editform"
      action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/id/'.$id.'/'; ?>" 
      method="post" 
      onmousedown="hideCalendar();">
<input type="hidden" name="id" value="<?= htmlspecialchars($rubricator->id); ?>" />
<input type="hidden" name="id_search_template" value="<?= htmlspecialchars($rubricator->id_search_template); ?>" />
<input type="hidden" name="parent_id" value="<?=  id > 0 ?  htmlspecialchars($rubricator->id_parent): htmlspecialchars($parent_id) ?>" />
<input type="hidden" name="io_id" value="<?=  id > 0 ?  htmlspecialchars($rubricator->id_io_object): htmlspecialchars($io_id) ?>" />
<input type="hidden" name="code" value="<?= htmlspecialchars($rubricator->code); ?>" />
<input type="hidden" name="save" value="1" />

<table width="100%" cellpadding="2" cellspacing="2" border="0" class="formz" align="center">


<tr>
   

    <td valign="top" >
        <table border="0" cellpadding="2" cellspacing="2" width="450" class="column">
        <tr>
    <td align="right" valign="top" width="30%">Выбрать категорию:</td>
    <td width="60%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
           <tr>
                <td width="70%">
                    <select name="cat_id" class="wide">
                        <option value="0" />                        
                        <?
                            for ($i=0;$i<count($categories);$i++) {
                                ?>
                                    <option value="<?= $categories[$i]->id; ?>"<?=($rubricator->id_io_category==$categories[$i]->id ? ' selected="selected"':'');?> ><?= htmlspecialchars($categories[$i]->name); ?></option>
                                <?
                            }
                        ?>
                    </select>
                </td>
              
                <td><button id="select-employee" onclick="return SelectCategoryAction();" class="wide"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Выбрать">Выбрать...</button></td>
            	
                </tr>
        </table>
    </td>
 <tr>
    <td align="right" class="required">Наименование:</td>
    <td><input type="text" name="name" value="<?= htmlspecialchars($rubricator->name); ?>" class="wide" /></td>
</tr>        
		
<tr>
    <td align="right" class="required">Код:</td>
    <td><input type="text" name="code_m" value="<?= htmlspecialchars($rubricator->code); ?>" class="wide" disabled /></td>
</tr>



 
</tr>  





        </table>
    </td>
    <td>
        <table border="0" width="50" class="column">
        </table>
    </td>
    
<td valign="top" >
<table border="0" cellpadding="2" cellspacing="2" width="450" class="column">
       
              
   <td valign="top" >
        <table border="0" cellpadding="2" cellspacing="2" width="450" class="column">
        <tr>
    <td align="right" valign="top" width="30%">Выбрать поисковый шаблон:</td>
    <td width="60%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
           <tr>
                <td width="70%">
                    <select name="templ_id" class="wide">
                        <option value="0" />                        
                        <?
                            for ($i=0;$i<count($templates);$i++) {
                                ?>
                                    <option value="<?= $templates[$i]->id; ?>"<?=($rubricator->id_search_template==$templates[$i]->id ? ' selected="selected"':'');?> ><?= htmlspecialchars($templates[$i]->name); ?></option>
                                <?
                            }
                        ?>
                    </select>
                </td>
              
                <td><button id="select-employee" onclick="return SelectTemplateAction();" class="wide"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Выбрать">Выбрать...</button></td>
            	
                </tr>
        </table>
    </td>

<tr>
		<td align="right">Описание:</td>
              <td>  <textarea name="description" class="wide" rows="4"><?= htmlspecialchars($rubricator->description); ?></textarea>
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

function SelectCategoryAction()
{
    var url = '<?= SITE_HTTP_ROOT; ?>message/select-object/callback/SetKioAction';

    var w = 500;
    var h = 400;
    var param = '';
    OpenPopupWindow(url, w, h, param);

    return false;
}



function EditAction(id)
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
            ?>
                var url = '<?= SITE_HTTP_ROOT; ?>slovar/detail/id/' + id + '/';
                
                var w = 500;
    			var h = 400;
    			var param = '';
    			OpenPopupWindow(url, w, h, param);
    			
    			return false;
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
