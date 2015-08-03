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
        $submit_str = 'selectAllLists(';
        reset($variable_modules);
        $list_arr = array();
        while(list($variable_type, )=each($variable_modules)) {
            $list_arr[].="document.editform.elements['enabled_role_variables[".htmlspecialchars($variable_type)."][]']";
        }
        $submit_str .= join(', ', $list_arr).');';
        ?>
<form name="editform" id="editform" action="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/'; ?>" method="post" onsubmit="<?= $submit_str; ?> return true;">
<input type="hidden" name="id" value="<?= htmlspecialchars($role->id_role); ?>" />
<input type="hidden" name="callback" value="<?= htmlspecialchars($_REQUEST['callback']); ?>" />
<input type="hidden" name="save" value="1" />
<table width="100%" cellpadding="3" cellspacing="0" border="0" class="form">
<tr>
    <th colspan="2">Редактирование роли</th>
</tr>
<tr>
    <td align="right" class="required">Название:</td>
    <td width="100%"><input type="text" name="name" value="<?= htmlspecialchars($role->name); ?>" class="wide" /></td>
</tr>
<tr>
    <td colspan="2" valign="top">
        <span class="label">Частичный доступ к информации</span><br>
        <?
            reset($variable_modules);
            while(list($variable_type, $data)=each($variable_modules)) {
                ?>
                    <table cellpadding="0" cellspacing="0" border="0" width="100%" class="switchLists">
                        <tr>
                            <td colspan="3" class="subTitle">Данные модуля &quot;<?= htmlspecialchars($data['name']); ?>&quot;</td>
                        </tr>
                        <tr>
                            <td width="49%">
                                <select name="enabled_role_variables[<?= htmlspecialchars($variable_type); ?>][]" size="5" class="wide" multiple>
                                    <?
                                        for ($i=0;$i<count($data['vars']);$i++) {
                                            if (!key_exists($data['vars'][$i]->id_variable, $role_variables)) continue;
                                            ?>
                                                <option value="<?= $data['vars'][$i]->id_variable; ?>"><?= htmlspecialchars($data['vars'][$i]->show_name); ?></option>
                                            <?
                                        }
                                    ?>
                                </select>
                            </td>
                            <td valign="middle" align="center" width="1%">&nbsp;<input type="button" value="&gt;" onclick="moveOption(document.editform.elements['enabled_role_variables[<?= htmlspecialchars($variable_type); ?>][]'], document.editform.elements['disabled_role_variables[<?= htmlspecialchars($variable_type); ?>][]'])" class="arrowButton">&nbsp;<input type="button" value="&lt;" onclick="moveOption(document.editform.elements['disabled_role_variables[<?= htmlspecialchars($variable_type); ?>][]'], document.editform.elements['enabled_role_variables[<?= htmlspecialchars($variable_type); ?>][]'])" class="arrowButton">&nbsp;<br><br>&nbsp;<input type="button" value="&gt;&gt;" onclick="moveAllOptions(document.editform.elements['enabled_role_variables[<?= htmlspecialchars($variable_type); ?>][]'], document.editform.elements['disabled_role_variables[<?= htmlspecialchars($variable_type); ?>][]'])" class="arrowButton">&nbsp;<input type="button" value="&lt;&lt;" onclick="moveAllOptions(document.editform.elements['disabled_role_variables[<?= htmlspecialchars($variable_type); ?>][]'], document.editform.elements['enabled_role_variables[<?= htmlspecialchars($variable_type); ?>][]'])" class="arrowButton">&nbsp;</td>
                            <td width="49%">
                                <select name="disabled_role_variables[<?= htmlspecialchars($variable_type); ?>][]" size="5" class="wide" multiple>
                                    <?
                                        for ($i=0;$i<count($data['vars']);$i++) {
                                            if (key_exists($data['vars'][$i]->id_variable, $role_variables)) continue;
                                            ?>
                                                <option value="<?= $data['vars'][$i]->id_variable; ?>"><?= htmlspecialchars($data['vars'][$i]->show_name); ?></option>
                                            <?
                                        }
                                    ?>
                                </select>
                            </td>
                        </tr>
                    </table>
                <?
            }
        ?>
    </td>
</tr>
<tr>
    <td colspan="2" valign="top">
        <span class="label">Доступ к модулям</span><br>
        <table cellpadding="0" cellspacing="0" width="100%" class="coloredList">
            <tr>
                <th width="100%">Доступ</th>
                <th class="center">разрешен</th>
                <th class="center">запрещен</th>
            </tr>
            <?
                for ($i=0;$i<count($modules);$i++) {
                    ?>
                        <tr>
                            <td colspan="3" class="subTitle"><?= htmlspecialchars($modules[$i]->name); ?></td>
                        </tr>
                    <?
                    reset($module_actions);
                    for ($j=0;$j<count($module_actions[$modules[$i]->id_module]);$j++) {

                        $enable = false;
                        if (key_exists($module_actions[$modules[$i]->id_module][$j]->id_module_action, $role_actions)) $enable = true;
                        ?>
                            <tr>
                                <td><?= htmlspecialchars($module_actions[$modules[$i]->id_module][$j]->name); ?></td>
                                <td class="center"><input type="radio" name="enabled_action[<?= $module_actions[$modules[$i]->id_module][$j]->id_module_action; ?>]" value="1"<?= $enable ? ' checked':''; ?>></td>
                                <td class="center"><input type="radio" name="enabled_action[<?= $module_actions[$modules[$i]->id_module][$j]->id_module_action; ?>]" value="0"<?= !$enable ? ' checked':''; ?>></td>
                            </tr>
                        <?
                    }
                }
            ?>
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