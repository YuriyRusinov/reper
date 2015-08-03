<?
    if ($save && !$errors) {
        ?>
<script language="JavaScript">
<!--
    var cmd = 'window.opener.<?= $_REQUEST['callback']; ?>(<?= $department->id_department; ?>, <?= intval($department->parent_id); ?>, "edit", "<?= escape($department->name); ?>");';
    eval(cmd);
    self.close();
//-->
</script>
        <?
    } else {
        ?>
<form name="editform" id="editform" action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" method="post">
<input type="hidden" name="id" value="<?= htmlspecialchars($department->id_department); ?>" />
<input type="hidden" name="parent_id" value="<?= htmlspecialchars($department->parent_id); ?>" />
<input type="hidden" name="callback" value="<?= htmlspecialchars($_REQUEST['callback']); ?>" />
<input type="hidden" name="save" value="1" />
<table width="100%" cellpadding="3" cellspacing="0" border="0" class="form">
<tr>
    <th colspan="2">Редактирование данных подразделения</th>
</tr>
<tr>
    <td align="right">Идентификатор:</td>
    <td width="100%"><?= htmlspecialchars($department->id_department); ?></td>
</tr>
<tr>
    <td align="right" class="required">Название:</td>
    <td><input type="text" name="name" value="<?= htmlspecialchars($department->name); ?>" class="wide" /></td>
</tr>
<tr>
    <td align="right" valign="top">Начальник:</td>
    <td width="100%" class="tableIn">
        <table cellpadding="0" cellspacing="0" width="100%" border="0">
            <tr>
                <td width="70%">
                    <select name="head_id" class="wide">
                        <option value="" />                        
                        <?
                            for ($i=0;$i<count($employees);$i++) {
                                ?>
                                    <option value="<?= $employees[$i]->id_employee; ?>"<?=($department->head_id==$employees[$i]->id_employee ? ' selected="selected"':'');?> ><?= htmlspecialchars($employees[$i]->name); ?></option>
                                <?
                            }
                        ?>
                    </select>
                </td>
                <td><button id="select-employee" onclick="return SelectAction();" class="wide"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Выбрать начальника">Выбрать...</button></td>
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td align="right" class="required" nowrap>Условный номер:</td>
    <td><input type="text" name="prefix" value="<?= htmlspecialchars($department->prefix); ?>" class="wide" /></td>
</tr>
<tr>
    <td colspan="2">Описание:<br>
        <textarea name="description" rows="5" class="wide"><?= htmlspecialchars($department->description); ?></textarea>
    </td>
</tr>

<tr>
    <td colspan="2" class="line" />
</tr>
<tr>
    <td  align="center" colspan="2"><input type="submit" name="save" value="Сохранить" class="button" /></td>
</tr>
</table>
</form>

<script language="JavaScript">
<!--
function SetAction(value, text)
{
    var list = document.editform.elements['head_id'];

    selectOption(list, value);
}

function SelectAction()
{
    var url = '<?= SITE_HTTP_ROOT; ?>employee/select-employee/callback/SetAction';

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