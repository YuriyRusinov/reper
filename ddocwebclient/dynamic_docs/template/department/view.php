<link rel="STYLESHEET" type="text/css" href="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxtree.css">
<script  src="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxcommon.js"></script>
<script  src="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxtree.js"></script>
<script  src="<?= SITE_URI_ROOT; ?>js/dhtmlx/ext/dhtmlxtree_xw.js"></script>
<script  src="<?= SITE_URI_ROOT; ?>js/dhtmlx/ext/dhtmlxtree_my.js"></script>

<div id="department-manipulation">
    <div id="department-toolbar" class="toolbar">
        <button id="add-department" onclick="EditAction('new')"><img src="<?= SITE_URI_ROOT; ?>img/iconNew.gif" title="Добавить новое подразделение">Добавить</button>
        <button id="edit-department" onclick="EditAction('exist')"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Редактировать подразделение">Редактировать</button>
        <button id="delete-department" onclick="DeleteAction()"><img src="<?= SITE_URI_ROOT; ?>img/iconDelete.gif" title="Удалить выбраное подразделение">Удалить</button>
    </div>
    <div id="department-tree" class="tree"></div>
</div>

<script language="JavaScript">
<!--
function InitButtons(selected_node) {
    document.getElementById('add-department').disabled = true;
    document.getElementById('edit-department').disabled = true;
    document.getElementById('delete-department').disabled = true;

    if (selected_node==null) {
        return;
    }

    if (selected_node>=0) {
        <?
            if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
                ?>
                    document.getElementById('add-department').disabled = false;
                <?
            }
        ?>
    }

    if (selected_node>0) {
        <?
            if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
                ?>
                    document.getElementById('edit-department').disabled = false;
                <?
            }

            if (access_enable($STORAGE, $STORAGE['module'], 'delete')) {
                ?>
                    document.getElementById('delete-department').disabled = false;
                <?
            }
        ?>
    }
}

function EditAction(type) {
    var itemId = tree.getSelectedItemId();
    var parentId = tree.getParentId(itemId);

    if (type=='new') {
        itemId = 0;
        parentId = tree.getSelectedItemId();
    }

    var url = '<?= SITE_HTTP_ROOT; ?>department/edit/id/' + itemId + '/parent_id/' + parentId + '/callback/Refresh';
    var w = 500;
    var h = 200;
    var param = '';
    OpenPopupWindow(url, w, h, param);
}

function DeleteAction() {
    var itemId = tree.getSelectedItemId();

    if (tree.hasChildren(itemId)>0) {
        alert('Запрещается удалять ветки, содержащие вложения.');
        return;
    }

    if (confirm('После проведения операции, восстановление будет невозможно. Вы действительно хотите продолжить?')) {
        var parentId = tree.getParentId(itemId);
        var url = '<?= SITE_HTTP_ROOT; ?>department/delete/id/' + itemId + '/parent_id/' + parentId + '/callback/Refresh';
        var w = 1;
        var h = 1;
        var param = '';
        OpenPopupWindow(url, w, h, param);
    }
}

function Edit(itemId) {
    InitButtons(itemId);
    if (itemId > 0) {
        EditAction('exist');
    }
}

function Move(itemId, toId) {
    if (itemId!=toId) {
        var url = '<?= SITE_HTTP_ROOT; ?>department/move/id/' + itemId + '/parent_id/' + toId + '/callback/Refresh';
        var w = 1;
        var h = 1;
        var param = '';
        OpenPopupWindow(url, w, h, param);
    }
}

function Refresh(itemId, parentId, action, name) {
    if (action=='delete') {
        tree.deleteItem(itemId, false)
        var sel = tree.getItemIdByIndex(parentId, 0)
        if (sel==undefined) {
            tree.selectItem(parentId, true, false);
        } else {
            tree.selectItem(sel, true, false);
        }
    }

    if (action=='edit') {
        if (tree.getIndexById(itemId)==null) {
            tree.insertNewChild(parentId,itemId,name)
        }
        tree.setItemText(itemId, name);
    }

    if (tree.getSelectedItemId()!='') {
        InitButtons(tree.getSelectedItemId());
    } else {
        InitButtons(null);
    }
}

//-->
</script>

<script language="JavaScript">
<!--
tree=new dhtmlXTreeObject("department-tree","100%","100%","root");
tree.attachEvent("onClick", InitButtons);

<?
if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
    ?>
        tree.attachEvent("onDblClick", Edit);
        tree.attachEvent("onDrop", Move);
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
tree.setXMLAutoLoading("<?= SITE_HTTP_ROOT; ?>directory.php?module=department&view=tree");
tree.attachEvent("onXLE", RestoreTree);
tree.loadXML("<?= SITE_HTTP_ROOT; ?>directory.php?module=department&view=tree&id=root");

InitButtons(null);

function RestoreTree(treeObj)
{
    treeObj.attachEvent("onXLE", null);
    treeObj.loadOpenStates('department-view-tree');
}
window.onunload = new Function('tree.saveOpenStates("department-view-tree");');


//-->
</script>
