(defun msdos-to-unix-file()
  "make line ends with \n"
  (interactive)
  (flymake-mode-off)
  (goto-char (point-min))
  (while (search-forward "\r\n" nil t)
    (replace-match "\n"))
  (flymake-mode-on))

(require 'session)
(add-hook 'after-init-hook 'session-initialize)

(load "desktop")
(require 'desktop)
(desktop-load-default)
(add-hook 'after-init-hook 'desktop-read)
(add-hook 'kill-emacs-hook (lambda () (desktop-save "~/.emacs.d/")))

(defun lq-filter (condp lst)
  (delq nil
	(mapcar (lambda (x) (and (funcall condp x) x)) lst)))

(defun parent-dir (dir)
  (replace-regexp-in-string "\\/[^/]*$" "" dir))

(provide 'onlyu-util)
