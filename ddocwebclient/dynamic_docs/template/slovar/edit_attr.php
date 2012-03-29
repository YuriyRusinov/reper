<?
    if ($save && !$errors) {
    	
    	
    	
        ?>
<script language="JavaScript">
<!--
   
    
   <?
   $val = '';
     for ($i=0;$i<count($list_values);$i++) {
                                
            if( $list_values[$i]['id']==$value)
              {
             		$val = $list_values[$i][$column_name];
               		break;
              }
                                
        }
    
    
    ?>
     var cmd = 'window.opener.<?= $_REQUEST['callback']; ?>(<?=$attr_id;?>,"<?=$val;?>");';
    
    eval(cmd);
    self.close();
//-->
</script>
        <?
    } else {
        ?>
<form name="editform" id="editform" action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" method="post" onsubmit="return true;">
<input type="hidden" name="id" value="<?= htmlspecialchars($attr->id_io_object); ?>" />
<input type="hidden" name="attr_id" value="<?= htmlspecialchars($attr->id_io_attribute); ?>" />
<input type="hidden" name="callback" value="<?= htmlspecialchars($_REQUEST['callback']); ?>" />
<input type="hidden" name="save" value="1" />
<table width="100%" cellpadding="3" cellspacing="0" border="0" class="form">
<tr>
    <th colspan="2">Редактирование атрибута</th>
</tr>
<tr>
    <td align="right">ID:</td>
    <?

    
    $attr_name = '';
    
    for($j=0;$j<count($names);$j++)
    {
    	if($names[$j]->id == $attr->id_io_attribute)
    	{
    		
    		$attr_name = $names[$j]->name;
    		break;
    	}
    	
    		
    }
   
    ?>
    
    
    <td width="100%"><?= htmlspecialchars($attr_name); ?></td>
</tr>

<?
 if($type_attribute == 2)
 {
?>

 <tr>
    <td align="right" valign="top">Значение:</td>
    <td width="100%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
            <tr>
                <td width="70%">
                 <select name="value" class="wide" >                      
                        <?
                            for ($i=0;$i<count($list_values);$i++) {
                                ?>
                                    <option value="<?= $list_values[$i]['id']; ?>"<?=($list_values[$i]['id']==$attr->value ? ' selected="selected"':'');?> ><?= htmlspecialchars($list_values[$i][$column_name]); ?></option>
                                <?
                            }
                        ?>
                    </select>
                </td>
                
            </tr>
        </table>
    </td>
</tr>


<?
 }
?>

<?
 if($type_attribute == 9 || $type_attribute == 14)
 {
?>
<tr>
    <td class="required" align="right">Значение:</td>
    <td><input type="text" name="value" value="<?= htmlspecialchars($attr->value); ?>" class="wide" /></td>
</tr>
<?
 }
?>

<?
 if($type_attribute == 8 || $type_attribute == 6)
 {
?>
<tr>
    <td class="required" align="right">Значение:</td>
    <td><input type="text" name="value" value="<?= htmlspecialchars($attr->value); ?>" class="half" /></td>
</tr>
<?
 }
?>
<?
 if($type_attribute == 13 )
 {
?>
<tr>
    <td class="required" align="right">Значение:</td>
    <td>  <textarea name="value" class="wide" rows="4"><?= htmlspecialchars($attr->value); ?></textarea>
    </td>
</tr>
<?
 }
?>






<tr>
    <td colspan="2" class="line" />
</tr>
<tr>
    <td  align="center" colspan="2"><input type="submit" name="button" value="Сохранить" class="button" /></td>
</tr>
</table>
</form>
<script type="text/javascript" language="JavaScript">
<!--
function AddAction(value, text)
{
    var list = document.editform.elements['contact_persons[]'];
    addOption(list, value, text);
}

function SelectAction()
{
    var url = '<?= SITE_HTTP_ROOT; ?>contact-person/select-contact-person/callback/AddAction';

    var w = 550;
    var h = 200;
    var param = '';
    OpenPopupWindow(url, w, h, param);

    return false;
}

function DeleteAction() {
    var list = document.editform.elements['contact_persons[]'];

    deleteSelectedOption(list);

    return false;
}
//-->
</script>
        <?
    }
?>