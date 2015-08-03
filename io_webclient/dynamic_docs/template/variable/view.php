<?= $page_break; ?>
<div id="variable-toolbar" class="toolbar">
    <button id="edit-variable-0" onclick="EditAction(0)"><img src="<?= SITE_URI_ROOT; ?>img/iconNew.gif" title="Добавить дополнительное поле">Добавить</button>
</div>
<table cellpadding="3" cellspacing="1" border="0" class="list">
    <tr>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/from/0/sort/id_variable/sort_dir/<?= $sort=='id_variable' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>">ID</a><?= $sort=='id_variable' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/from/0/sort/order/sort_dir/<?= $sort=='order' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>">№ п/п</a><?= $sort=='order' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/from/0/sort/name/sort_dir/<?= $sort=='name' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>">Название</a><?= $sort=='name' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/from/0/sort/show_name/sort_dir/<?= $sort=='show_name' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>">Надпись</a><?= $sort=='show_name' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/from/0/sort/module/sort_dir/<?= $sort=='module' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>">Раздел</a><?= $sort=='module' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <th>Операции</th>
    </tr>
    <?
        $odd=false;
        for ($i=0; $i<count($variables->_collection); $i++) {
            $odd=!$odd;
            ?>
                <tr <?=($odd?'class="odd"':'')?>>
                    <td class="center"><?= $variables->_collection[$i]->id_variable; ?></td>
                    <td class="center"><?= $variables->_collection[$i]->order; ?></td>
                    <td><?= htmlspecialchars($variables->_collection[$i]->name); ?></td>
                    <td><?= htmlspecialchars($variables->_collection[$i]->show_name); ?></td>
                    <td><?= htmlspecialchars($variables->_collection[$i]->GetModuleName()); ?></td>
                    <td>
                        <button id="edit-variable-<?= $variables->_collection[$i]->id_variable ?>" onclick="EditAction(<?= $variables->_collection[$i]->id_variable ?>)"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Редактировать дополнительное поле">Редактировать</button>
                        <button id="delete-variable-<?= $variables->_collection[$i]->id_variable ?>" onclick="DeleteAction(<?= $variables->_collection[$i]->id_variable ?>)"><img src="<?= SITE_URI_ROOT; ?>img/iconDelete.gif" title="Удалить дополнительное поле">Удалить</button>
                    </td>
                </tr>
            <?
        }
    ?>
</table>

<script language="JavaScript">
<!--
function InitButtons()
{
    var buttons = document.getElementsByTagName('button');
    var i = 0;
    var id = '';

    for (i=0; i<buttons.length; i++) {
        id = buttons[i].id;
        if (id.indexOf("edit-variable-")>=0) {
            <?
                if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
                    ?>
                        document.getElementById(id).disabled = false;
                    <?
                } else {
                    ?>
                        document.getElementById(id).disabled = true;
                    <?
                }
            ?>
        }

        if (id.indexOf("delete-variable-")>=0) {
            <?
                if (access_enable($STORAGE, $STORAGE['module'], 'delete')) {
                    ?>
                        document.getElementById(id).disabled = false;
                    <?
                } else {
                    ?>
                        document.getElementById(id).disabled = true;
                    <?
                }
            ?>
        }
    }
}

function EditAction(id)
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
            ?>
                var w = 350;
                var h = 200;
                var param = '';
                var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/edit/id/' + id + '/callback/Refresh/';
                OpenPopupWindow(url, w, h, param);
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}


function DeleteAction(id)
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
            ?>
                if (confirm('После проведения операции, восстановление будет невозможно. Вы действительно хотите продолжить?')) {
                    var w = 1;
                    var h = 1;
                    var param = '';
                    var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/delete/id/' + id + '/callback/Refresh/';
                    OpenPopupWindow(url, w, h, param);
                }
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}

function Refresh()
{
    var offsetX = document.body.scrollLeft;
    var offsetY = document.body.scrollTop;
    var location = "<?= SITE_HTTP_ROOT.preg_replace('/\/x\/\d+\/y\/\d+/', '', $STORAGE['request_uri']); ?>";

    window.location = location + 'x/' + offsetX + '/y/' + offsetY;
}

InitButtons();

//-->
</script>
