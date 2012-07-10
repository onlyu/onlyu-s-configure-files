(defun msdos-to-unix-file()
  "make line ends with \n"
  (interactive)
  (goto-char (point-min))
  (while (search-forward "\r\n" nil t)
    (replace-match "\n")))

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


(provide 'onlyu-util)