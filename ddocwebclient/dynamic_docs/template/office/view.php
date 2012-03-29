<?= $page_break; ?>
<div id="office-toolbar" class="toolbar">
    <button id="edit-office-0" onclick="EditAction(0)"><img src="<?= SITE_URI_ROOT; ?>img/iconNew.gif" title="Добавить должность" />Добавить</button>
</div>
<table cellpadding="3" cellspacing="1" border="0" class="list">
    <tr>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/from/0/sort/id_office/sort_dir/<?= $sort=='id_office' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>">ID</a><?= $sort=='id_office' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <th><a href="<?= SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action']; ?>/from/0/sort/name/sort_dir/<?= $sort=='name' ? $convert_sort_dirs[$sort_dir]:'asc'; ?>">Название</a><?= $sort=='name' ? ($sort_dir=='asc' ? '▲':'▼'):''; ?></th>
        <th>Операции</th>
    </tr>
    <?
        $odd=false;
        for ($i=0; $i<count($offices); $i++) {
            $odd=!$odd;
            ?>
                <tr <?=($odd?'class="odd"':'')?>>
                    <td class="center"><?= $offices[$i]['id_office']; ?></td>
                    <td><?= htmlspecialchars($offices[$i]['name']); ?></td>
                   
                    <td>
                        <button id="edit-office-<?= $offices[$i]['id_office'] ?>" onclick="EditAction(<?= $offices[$i]['id_office'] ?>)"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Редактировать данные контрагента" />Редактировать</button>
                        <button id="delete-office-<?= $offices[$i]['id_office'] ?>" onclick="DeleteAction(<?= $offices[$i]['id_office'] ?>)"><img src="<?= SITE_URI_ROOT; ?>img/iconDelete.gif" title="Удалить данные контрагента" />Удалить</button>
                    </td>
                </tr>
            <?
        }
    ?>
</table>

<script type="text/javascript" language="JavaScript">
<!--
function InitButtons()
{
    var buttons = document.getElementsByTagName('button');
    var i = 0;
    var id = '';

    for (i=0; i<buttons.length; i++) {
        id = buttons[i].id;

        if (id.indexOf("edit-office-")>=0) {
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

        if (id.indexOf("delete-office-")>=0) {
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
                var w = 450;
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
