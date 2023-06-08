<?php
// リダイレクト先のページのパス
$redirect_path = "/_php/test.php";

// リダイレクトを実行
header("Location: " . $redirect_path);
exit;
?>
