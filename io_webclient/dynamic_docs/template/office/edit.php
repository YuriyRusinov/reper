<?
    if ($save && !$errors) {
        ?>
<script language="JavaScript">
<!--
    var cmd = 'window.opener.<?= $_REQUEST['callback']; ?>();';
    eval(cmd);
    self.close();
//-->
</script>
        <?
    } else {
        ?>
<form name="editform" id="editform" action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" method="post" onsubmit="selectAllLists(document.editform.elements['contact_persons[]']); return true;">
<input type="hidden" name="id" value="<?= htmlspecialchars($office->id_office); ?>" />
<input type="hidden" name="callback" value="<?= htmlspecialchars($_REQUEST['callback']); ?>" />
<input type="hidden" name="save" value="1" />
<table width="100%" cellpadding="3" cellspacing="0" border="0" class="form">
<tr>
    <th colspan="2">Редактирование должности</th>
</tr>
<tr>
    <td align="right">ID:</td>
    <td width="100%"><?= htmlspecialchars($office->id_office); ?></td>
</tr>
<tr>
    <td class="required" align="right">Название:</td>
    <td><input type="text" name="name" value="<?= htmlspecialchars($office->name); ?>" class="wide" /></td>
</tr>

<tr>
    <td class="required" align="right">Комментарий:</td>
    <td><input type="text" name="remark" value="<?= htmlspecialchars($office->remark); ?>" class="wide" /></td>
</tr>


<tr>
    <td colspan="2" class="line" />
</tr>
<tr>
    <td  align="center" colspan="2"><input type="submit" name="save" value="Сохранить" class="button" /></td>
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