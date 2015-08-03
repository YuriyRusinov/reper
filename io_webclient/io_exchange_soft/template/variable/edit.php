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
<form action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" method="post">
<input type="hidden" name="id" value="<?= htmlspecialchars($variable->id_variable); ?>" />
<input type="hidden" name="callback" value="<?= htmlspecialchars($_REQUEST['callback']); ?>" />
<input type="hidden" name="save" value="1" />
<table width="100%" cellpadding="3" cellspacing="0" border="0" class="form">
<tr>
    <th colspan="2">Редактирование дополнительного поля</th>
</tr>
<tr>
    <td align="right">ID:</td>
    <td width="100%"><?= htmlspecialchars($variable->id_variable); ?></td>
</tr>
<tr>
    <td align="right" class="required">Раздел:</td>
    <td>
        <select name="module" class="wide">
            <?
                $modules = $variable->GetModules();
                while(list($key, $val)=each($modules)) {
                    ?>
                        <option value="<?= htmlspecialchars($key); ?>"<?= $variable->module==$key ? ' selected':''; ?>><?= htmlspecialchars($val); ?></option>
                    <?
                }
            ?>
        </select>
    </td>
</tr>
<tr>
    <td class="required" align="right">Название:</td>
    <td><input type="text" name="name" value="<?= htmlspecialchars($variable->name); ?>" class="wide" /></td>
</tr>
<tr>
    <td align="right" class="required">Надпись:</td>
    <td><input type="text" name="show_name" value="<?= htmlspecialchars($variable->show_name); ?>" class="wide" /></td>
</tr>
<tr>
    <td align="right" class="required">Обязательное:</td>
    <td>
        <input type="radio" name="required" value="yes"<?= $variable->required=='yes' ? ' checked':''; ?>> да
        <input type="radio" name="required" value="no"<?= $variable->required=='no' ? ' checked':''; ?>> нет
    </td>
</tr>
<tr>
    <td align="right" class="required" nowrap>Выводить в списке:</td>
    <td>
        <input type="radio" name="list_show" value="yes"<?= $variable->list_show=='yes' ? ' checked':''; ?>> да
        <input type="radio" name="list_show" value="no"<?= $variable->list_show=='no' ? ' checked':''; ?>> нет
    </td>
</tr>
<tr>
    <td align="right" class="required">Номер по порядку:</td>
    <td><input type="text" name="order" value="<?= htmlspecialchars($variable->order); ?>" size="4" /></td>
</tr>
<tr>
    <td align="right" class="required">Тип:</td>
    <td>
        <select name="type" class="wide">
            <?
                $types = $variable->GetTypes();
                while(list($key, $val)=each($types)) {
                    ?>
                        <option value="<?= htmlspecialchars($key); ?>"<?= $variable->type==$key ? ' selected':''; ?>><?= htmlspecialchars($val); ?></option>
                    <?
                }
            ?>
        </select>
    </td>
</tr>
<tr>
    <td align="right" class="required">Формат:</td>
    <td><input type="text" name="format" value="<?= htmlspecialchars($variable->format); ?>" class="wide" /></td>
</tr>
<tr>
    <td align="right" nowrap>Сообщение об ошибке:</td>
    <td><input type="text" name="error_message" value="<?= htmlspecialchars($variable->error_message); ?>" class="wide" /></td>
</tr>
<tr>
    <td colspan="2" class="line" />
</tr>
<tr>
    <td  align="center" colspan="2"><input type="submit" name="save" value="Сохранить" class="button" /></td>
</tr>
</table>
</form>
        <?
    }
?>