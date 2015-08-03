<?
    if ($save && !$errors) {
        ?>
<script language="JavaScript">
<!--
    <?
        if ($_REQUEST['callback']!='') {
            ?>
                var cmd = 'window.opener.<?= $_REQUEST['callback']; ?>();';
                eval(cmd);
            <?
        }
     ?>
    self.close();
//-->
</script>
        <?
    } else {
        ?>
<form name="editform" id="editform" action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" method="post">
<input type="hidden" name="date" id="date" value="<?= htmlspecialchars($employee_date->date); ?>" />
<input type="hidden" name="callback" value="<?= htmlspecialchars($_REQUEST['callback']); ?>" />
<input type="hidden" name="save" value="1" />
<table width="100%" cellpadding="3" cellspacing="0" border="0" class="form">
<tr>
    <th colspan="3">Редактирование настроек дня</th>
</tr>
<tr>
    <td align="right" nowrap>Дата:</td>
    <td colspan="2">
        <?
            if ($employee_date->date!='') {
                $employee_date_str = format_date('{d}.{m}.{Y}', $employee_date->date);
                if ($employee_date_str=='00.00.0000') $employee_date_str = '';
            }
        ?>
        <?= htmlspecialchars($employee_date_str); ?>
    </td>
</tr>
<tr>
    <td align="right" nowrap>Статус дня:</td>
    <td colspan="2" width="100%">
        <select name="status" id="status" class="wide" onchange="SwitchStatus(this.value)">
            <?
                $statuses = $employee_date->GetStatuses();
                while(list($key, $val)=each($statuses)) {
                    ?>
                        <option value="<?= htmlspecialchars($key); ?>"<?= $employee_date->status==$key ? ' selected':''; ?>><?= htmlspecialchars($val); ?></option>
                    <?
                }
            ?>
        </select>
    </td>
</tr>
<tr id="start_work_row">
    <td align="right" nowrap>Начало работы:</td>
    <td colspan="2" width="100%"><input type="text" name="start_work" value="<?= htmlspecialchars($start_work); ?>" class="time readonly" readonly></td>
</tr>
<tr id="stop_work_row">
    <td align="right" nowrap>Окончание работы:</td>
    <td colspan="2" width="100%"><input type="text" name="stop_work" value="<?= htmlspecialchars($stop_work); ?>" class="time"></td>
</tr>
<tr>
    <td colspan="3" nowrap>Описание:<br>
        <textarea class="wide" name="description" rows="5"><?= htmlspecialchars($employee_date->description); ?></textarea>
    </td>
</tr>

<tr>
    <td colspan="3" class="line" />
</tr>
<tr>
    <td  align="center" colspan="3"><input type="submit" name="save" value="Сохранить" class="button" /></td>
</tr>
</table>
</form>


<script language="JavaScript">
<!--

function SwitchStatus(status)
{
    if (status=='work') {
        ShowElement('start_work_row');
        ShowElement('stop_work_row');
    } else {
        HideElement('start_work_row');
        HideElement('stop_work_row');
    }
}

SwitchStatus(document.getElementById('status').value);

//-->
</script>
        <?
    }
?>