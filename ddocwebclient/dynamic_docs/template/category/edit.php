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
    	      
?>

<div class="pagename">
    Редактирование категории
</div>
<form name="editform" 
      id="editform"
      action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/id/'.$id.'/'; ?>" 
      method="post" 
      onmousedown="hideCalendar();">
<input type="hidden" name="id" value="<?= htmlspecialchars($category->id); ?>" />
<input type="hidden" name="is_main" value="<?= htmlspecialchars($category->is_main); ?>" />
<input type="hidden" name="parent_id" value="<?= htmlspecialchars($parent_id); ?>" />
<input type="hidden" name="save" value="1" />

<table width="100%" cellpadding="2" cellspacing="2" border="0" class="formz" align="center">
<tr>
    <td width="1%" colspan="3" >
    <table width="100%" class="column" cellpadding="0" cellspacing="0" border="0" >
    <tr>
        
        <td style="text-align:right;">ID:&nbsp;<?= htmlspecialchars($category->id); ?></td>
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
    <td align="right" valign="top">Категория И.О:</td>
    <td width="100%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
            <tr>
                <td width="70%">
                <?
                
               for($j=0;$j<count($cats);$j++)
               {
               		if($category->id_io_type == $cats[$j]->id)
               		{
               			$kio = $cats[$j]->name;
               			break;
               		}
               }
                ?>
                    <input type="text" name="kio" value="<?= htmlspecialchars($kio); ?>" class="wide" disabled />
                    <input type="hidden" name="kio_id" value="<?= $category->id_io_type; ?>" />
                
                    </td>
                <td><button id="select-department" onclick="return SelectKioAction();" class="wide"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Выбрать Категорию" />Выбрать...</button></td>
            </tr>
        </table>
    </td>
</tr>  
        		
<tr>
    <td align="right" valign="top">Подчиненная категория:</td>
    <td width="100%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
            <tr>
                <td width="70%">
                    <select name="select_child" class="wide" <?= ($id===0 || $is_main == false)? 'disabled':''?> >
                        <option value="" />                        
                        <?
                            for ($i=0;$i<count($childs);$i++) {
                                ?>
                                    <option value="<?= $childs[$i]->id; ?>"<?=(($category->id_child==$childs[$i]->id ) ? ' selected="selected"':'');?> ><?= htmlspecialchars($childs[$i]->name); ?></option>
                                <?
                            }
                        ?>
                    </select>
                </td>
                <td><button id="select-child" <?= ($id===0 || $is_main == false)? 'disabled':'' ?> onclick="return SelectChildAction();" class="wide"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Выбрать">Выбрать...</button></td>
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
    <td align="right" class="required">Наименование:</td>
    <td><input type="text" name="name" value="<?= htmlspecialchars($category->name); ?>" class="wide" /></td>
</tr>        
		
<tr>
    <td align="right" class="required">Код:</td>
    <td><input   <?= ($id > 0 )? 'disabled':'' ?>    type="text" name="code" value="<?= htmlspecialchars($category->code); ?>" class="wide" /></td>
</tr>

<tr>
		<td align="right">Описание:</td>
              <td>  <textarea name="description" class="wide" rows="4"><?= htmlspecialchars($category->description); ?></textarea>
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
    document.editform.elements['kio'].value = text;
    document.editform.elements['kio_id'].value = value;
    
    
    for(var i=0;i<this.categor.length;i++)
    {
    	if(value == categor[i])
    	{
    		document.editform.element[select_child].disable = false;
    	}
    	
    }
    
    
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

function SelectChildAction()
{
    var url = '<?= SITE_HTTP_ROOT; ?>journal/select-kio/callback/SetAction';

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
