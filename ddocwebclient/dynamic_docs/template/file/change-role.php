<?
    if ($save && !$errors) {
        ?>
<script language="JavaScript">
<!--
    self.close();
//-->
</script>
        <?
    } else {
        ?>
<form name="editform" action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" method="post" enctype="multipart/form-data" onsubmit="selectAllLists(document.editform.elements['enabled_file_roles[]']); return true;">
<input type="hidden" name="destination" value="<?= htmlspecialchars($destination); ?>" />
<input type="hidden" name="callback" value="<?= htmlspecialchars($_REQUEST['callback']); ?>" />
<input type="hidden" name="save" value="1" />
<table width="100%" cellpadding="3" cellspacing="0" border="0" class="form">
<tr>
    <th colspan="2">Установка прав доступа</th>
</tr>
<tr>
    <td align="right" class="required" nowrap>Имя файла:</td>
    <td width="100%"><input type="text" name="name" value="<?= htmlspecialchars($file->name); ?>" class="wide" disabled/></td>
</tr>
<tr>
    <td colspan="2" valign="top">
        <span class="label required">Доступно ролям:</span><br>
        <table cellpadding="0" cellspacing="0" border="0" width="100%" class="switchLists">
            <tr>
                <td width="49%">
                    <select name="enabled_file_roles[]" size="5" class="wide" multiple>
                        <?
                            for ($i=0;$i<count($roles);$i++) {
                                if (!key_exists($roles[$i]->id_role, $enabled_file_roles)) continue;
                                    ?>
                                        <option value="<?= $roles[$i]->id_role; ?>"><?= htmlspecialchars($roles[$i]->name); ?></option>
                                    <?
                                }
                        ?>
                    </select>
                </td>
                <td valign="middle" align="center" width="1%">&nbsp;<input type="button" value="&gt;" onclick="moveOption(document.editform.elements['enabled_file_roles[]'], document.editform.elements['disabled_roles[]'])" class="arrowButton">&nbsp;<input type="button" value="&lt;" onclick="moveOption(document.editform.elements['disabled_roles[]'], document.editform.elements['enabled_file_roles[]'])" class="arrowButton">&nbsp;<br><br>&nbsp;<input type="button" value="&gt;&gt;" onclick="moveAllOptions(document.editform.elements['enabled_file_roles[]'], document.editform.elements['disabled_roles[]'])" class="arrowButton">&nbsp;<input type="button" value="&lt;&lt;" onclick="moveAllOptions(document.editform.elements['disabled_roles[]'], document.editform.elements['enabled_file_roles[]'])" class="arrowButton">&nbsp;</td>
                <td width="49%">
                    <select name="disabled_roles[]" size="5" class="wide" multiple>
                        <?
                            for ($i=0;$i<count($roles);$i++) {
                                if (key_exists($roles[$i]->id_role, $enabled_file_roles)) continue;
                                    ?>
                                        <option value="<?= $roles[$i]->id_role; ?>"><?= htmlspecialchars($roles[$i]->name); ?></option>
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