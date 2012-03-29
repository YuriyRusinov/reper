<?
    if (!$errors) {
?>
<script language="JavaScript">
<!--
    var cmd = 'window.opener.<?= $_REQUEST['callback']; ?>(<?= $department->id_department; ?>, <?= intval($department->parent_id); ?>, "delete");';
    eval(cmd);
    self.close();
//-->
</script>
<?
    }
?>