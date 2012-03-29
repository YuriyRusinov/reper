<?
    if (!$errors) {
?>
<script language="JavaScript">
<!--
    var cmd = 'window.opener.<?= $_REQUEST['callback']; ?>(<?= $rubricator->id; ?>, <?= intval($rubricator->id_parent); ?>, "move");';
    eval(cmd);
    self.close();
//-->
</script>
<?
    }
?>