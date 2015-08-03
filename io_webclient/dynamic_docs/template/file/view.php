<link rel="STYLESHEET" type="text/css" href="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxtree.css">
<script  src="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxcommon.js"></script>
<script  src="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxtree.js"></script>
<script  src="<?= SITE_URI_ROOT; ?>js/dhtmlx/ext/dhtmlxtree_xw.js"></script>
<script  src="<?= SITE_URI_ROOT; ?>js/dhtmlx/ext/dhtmlxtree_my.js"></script>

<div id="files" class="labeledTree">
    <fieldset>
        <legend>Файлы</legend>

        <div id="file-toolbar" class="toolbar">
            <?
                if (access_enable($STORAGE, $STORAGE['module'], 'upload')) {
                    ?>
                        <button id="add-file" onclick="return AddFileAction();"><img src="<?= SITE_URI_ROOT; ?>img/iconNew.gif" title="Добавить файл">Добавить файл</button>
                    <?
                } else {
                    ?>
                        <button id="add-file" disabled><img src="<?= SITE_URI_ROOT; ?>img/iconNew.gif" title="Добавить файл">Добавить файл</button>
                    <?
                }

                if (access_enable($STORAGE, $STORAGE['module'], 'add-directory')) {
                    ?>
                        <button id="add-directory" onclick="return AddDirAction();"><img src="<?= SITE_URI_ROOT; ?>img/iconNew.gif" title="Добавить директорию">Добавить директорию</button>
                    <?
                } else {
                    ?>
                        <button id="add-directory" diabled><img src="<?= SITE_URI_ROOT; ?>img/iconNew.gif" title="Добавить директорию">Добавить директорию</button>
                    <?
                }

                if (access_enable($STORAGE, $STORAGE['module'], 'change-role')) {
                    ?>
                        <button id="access" onclick="return ChangeRoleAction();"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Изменить доступ">Доступ</button>
                    <?
                } else {
                    ?>
                        <button id="access" diabled><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Изменить доступ">Доступ</button>
                    <?
                }

                if (access_enable($STORAGE, $STORAGE['module'], 'rename')) {
                    ?>
                        <button id="rename" onclick="return RenameAction();"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Переименовать файл/директорию">Переименовать</button>
                    <?
                } else {
                    ?>
                        <button id="rename" diabled><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Переименовать файл/директорию">Переименовать</button>
                    <?
                }

                if (access_enable($STORAGE, $STORAGE['module'], 'delete')) {
                    ?>
                        <button id="delete" onclick="return DeleteAction();" disabled><img src="<?= SITE_URI_ROOT; ?>img/iconDelete.gif" title="Удалить файл/директорию">Удалить</button>
                    <?
                } else {
                    ?>
                        <button id="delete" disabled><img src="<?= SITE_URI_ROOT; ?>img/iconDelete.gif" title="Удалить файл/директорию">Удалить</button>
                    <?
                }
            ?>
        </div>
        <div id="file-tree" class="visible tree"></div>
    </fieldset>
</div>

<script language="JavaScript">
<!--

function AddFileAction()
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'upload')) {
            ?>
                var itemId = tree.getSelectedItemId();
                var url = '<?= SITE_HTTP_ROOT; ?>alternative.php?module=file&action=upload&destination=' + urlencode(itemId) + '&callback=RefreshParent';
                var w = 450;
                var h = 200;
                var param = '';
                OpenPopupWindow(url, w, h, param);
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}

function ChangeRoleAction()
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'change-role')) {
            ?>
                var itemId = tree.getSelectedItemId();
                var url = '<?= SITE_HTTP_ROOT; ?>alternative.php?module=file&action=change-role&destination=' + urlencode(itemId);
                var w = 450;
                var h = 200;
                var param = '';
                OpenPopupWindow(url, w, h, param);
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}


function AddDirAction()
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'add-directory')) {
            ?>
                var itemId = tree.getSelectedItemId();
                var pattern = /[\/\\]+/g;

                var str=prompt("Введите имя директории", name);
                if (str!=null) {
                    while(str!=null && str.match(pattern)) {
                        if (str.match(pattern)) alert('Имя не должно содержать слеши');
                        str=prompt("Введите имя директории", "");
                    }

                    if (str!=null) {

                        var url = '<?= SITE_HTTP_ROOT; ?>alternative.php?module=file&action=add-directory&dir=' + urlencode(str) + '&destination=' + urlencode(itemId) + '&callback=RefreshParent';
                        var w = 1;
                        var h = 1;
                        var param = '';
                        OpenPopupWindow(url, w, h, param);
                    }
                }
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}


function CallbackRename(oldId, newId)
{
    var tree = current_tree;
    var tmp = newId.split('/');
    var name = tmp[tmp.length-1];

    if (name=='') name = tmp[tmp.length-2];

    tree.changeItemId(oldId, newId);
    tree.setItemText(newId,name)

    InitButtons(tree.getSelectedItemId());
}


function RenameAction()
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'rename')) {
            ?>
                var itemId = tree.getSelectedItemId();
                var pattern = /[\/\\]+/g;
                var tmp = itemId.split('/');
                var name = '';
                var i = 0;

                i = tmp.length-1;
                name = tmp[tmp.length-1];
                if (name=='') {
                    i = tmp.length-2
                    name = tmp[tmp.length-2];
                }


                var str=prompt("Введите новое имя", name);
                if (str!=null) {
                    while(str!=null && str.match(pattern)) {
                        if (str.match(pattern)) alert('Имя не должно содержать слеши');
                        str=prompt("Введите новое имя", "");
                    }

                    if (str!=null) {
                        tmp[i] = str;

                        var url = '<?= SITE_HTTP_ROOT; ?>alternative.php?module=file&action=rename&source=' + urlencode(itemId) + '&destination=' + urlencode(tmp.join('/')) + '&callback=CallbackRename';
                        var w = 1;
                        var h = 1;
                        var param = '';
                        OpenPopupWindow(url, w, h, param);
                    }
                }
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}

function CallbackDelete(itemId)
{
    var tree = current_tree;
    var parentId = tree.getParentId(itemId);
    var sel = tree.getItemIdByIndex(parentId, 0);

    tree.deleteItem(itemId, false);
    if (sel==undefined) {
        tree.selectItem(parentId, true, false);
    } else {
        tree.selectItem(sel, true, false);
    }

    InitButtons(tree.getSelectedItemId());
}

function DeleteAction()
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'delete')) {
            ?>
                var itemId = tree.getSelectedItemId();

                if (tree.hasChildren(itemId)>0) {
                    alert('Запрещается удалять ветки, содержащие вложения.');
                    return;
                }

                if (confirm('После проведения операции, восстановление будет невозможно. Вы действительно хотите продолжить?')) {
                    var url = '<?= SITE_HTTP_ROOT; ?>alternative.php?module=file&action=delete&source=' + urlencode(itemId) + '&callback=CallbackDelete';
                    var w = 1;
                    var h = 1;
                    var param = '';
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

function CheckMove(itemId, toId)
{
    var tree = current_tree;
    var itemParentId = tree.getParentId(itemId);
    var parentId = tree.getParentId(toId);

    if (toId=='root') return false;
    if (itemParentId=='root') return false;
    if (tree.getUserData(toId, 'isfile')=='1') return false;
    if (tree.getUserData(itemId, 'project_id')!=tree.getUserData(toId, 'project_id')) {
        alert('Перенос директорий и файлов разрешен только в пределах проекта');
        return false;
    }


    return true;
}


function InitButtons(itemId)
{
    var tree = current_tree;

    document.getElementById('add-file').disabled = true;
    document.getElementById('add-directory').disabled = true;
    document.getElementById('access').disabled = true;
    document.getElementById('delete').disabled = true;
    document.getElementById('rename').disabled = true;

    if (tree.getUserData(itemId, 'isfile')=='1' || tree.getUserData(itemId, 'isfile')=='0') {
        <?
            if (access_enable($STORAGE, $STORAGE['module'], 'delete')) {
                ?>
                    document.getElementById('delete').disabled = false;
                <?
            }
        ?>

        <?
            if (access_enable($STORAGE, $STORAGE['module'], 'rename')) {
                ?>
                    document.getElementById('rename').disabled = false;
                <?
            }
        ?>
    }

    if (itemId!=null && itemId!='root') {
        if (tree.getUserData(itemId, 'isfile')=='1') {
            <?
                if (access_enable($STORAGE, $STORAGE['module'], 'change-role')) {
                    ?>
                        document.getElementById('access').disabled = false;
                    <?
                }
            ?>
        } else {
            <?
                if (access_enable($STORAGE, $STORAGE['module'], 'upload')) {
                    ?>
                        document.getElementById('add-file').disabled = false;
                    <?
                }
            ?>

            <?
                if (access_enable($STORAGE, $STORAGE['module'], 'add-directory')) {
                    ?>
                        document.getElementById('add-directory').disabled = false;
                    <?
                }
            ?>
        }
    }
}


function ViewFile(itemId)
{
    var tree = current_tree;

    if (tree.getUserData(itemId, 'isfile')=='1') {
        today = new Date();
        WindowName='PopupWin'+today.getTime();
        var url = '<?= SITE_PROJECT_HTTP_ROOT; ?>'+  urlencode(itemId);
        window.open(url,WindowName);
    }
}

function RefreshNode()
{
    var tree = current_tree;
    tree.refreshItem(tree.getParentId(tree.getSelectedItemId()));

    InitButtons(tree.getSelectedItemId());
}

function RefreshParent(parentId)
{
    var tree = current_tree;
    tree.refreshItem(parentId);

    InitButtons(tree.getSelectedItemId());
}


function MoveItem(itemId, toId)
{
    <?
        if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
            ?>
                var w = 450;
                var h = 200;
                var param = '';
                var url = '<?= SITE_HTTP_ROOT; ?>alternative.php?module=file&action=move&source=' + urlencode(itemId) + '&destination=' + urlencode(toId) + '&callback=RefreshNode';
                OpenPopupWindow(url, w, h, param);
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}

tree=new dhtmlXTreeObject("file-tree","100%","100%","root");
tree.attachEvent("onClick", InitButtons);
tree.attachEvent("onDblClick", ViewFile);

<?
if (access_enable($STORAGE, 'file', 'edit')) {
    ?>
        tree.attachEvent("onDrag", CheckMove);
        tree.attachEvent("onDrop", MoveItem);
        tree.enableDragAndDrop(true);
        tree.enableImageDrag(true);
    <?
}
?>

tree.setImagePath("<?= SITE_URI_ROOT; ?>js/dhtmlx/imgs/csh_vista/");
tree.enableCheckBoxes(false);
tree.enableTreeLines(true);
tree.enableHighlighting(false);
tree.enableActiveImages(true);
tree.enableDragAndDropScrolling(true);
tree.setXMLAutoLoading("<?= SITE_HTTP_ROOT; ?>directory.php?module=file&view=tree");
tree.attachEvent("onXLE", RestoreTree);
tree.loadXML("<?= SITE_HTTP_ROOT; ?>directory.php?module=file&view=tree&id=root");

var current_tree = tree

InitButtons(null);

function RestoreTree(treeObj)
{
    treeObj.attachEvent("onXLE", null);
    treeObj.loadOpenStates('file-view-tree');
}
window.onunload = new Function('tree.saveOpenStates("file-view-tree");');

//-->
</script>