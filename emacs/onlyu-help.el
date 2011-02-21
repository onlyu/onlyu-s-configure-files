(provide 'onlyu-help)
(defun msdos-to-unix-file()
  "make line ends with \n"
  (interactive)
  (goto-char (point-min))
  (while (search-forward "\r\n" nil t)
    (replace-match "\n")))
