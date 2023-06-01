<?php
// リダイレクト先のページのパス
$redirect_path = "/new-page.php";

// リダイレクトを実行
header("Location: " . $redirect_path);
exit;
?>
