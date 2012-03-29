<?
    if (!$errors) {
?>
<script language="JavaScript">
<!--
    var cmd = 'window.opener.<?= $_REQUEST['callback']; ?>();';
    eval(cmd);
    self.close();
//-->
</script>
<?
    }
?>