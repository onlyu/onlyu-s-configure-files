;; face settings
(set-background-color "black")
(set-foreground-color "white")
(set-cursor-color "ivory3")
(set-mouse-color "white")
(load-file "~/emacs/lua-mode.el")
(load-file "~/emacs/psvn.el")
(load-file "~/emacs/xy2.el")
(load-file "~/emacs/fs/fs.el")

;; (load-file "~/.xemacs/font.el")
(setq auto-mode-alist (cons '("\\.lua$" . lua-mode) auto-mode-alist))
(autoload 'lua-mode "lua-mode" "Lua editing mode." t)

;; 显示行号									
;; (defun turn-on-line-number () (line-number-mode 1))
;; (add-hook 'text-mode-hook 'turn-on-line-number)
;; 显示鼠标的列号

;;显示行号
;;(load-file "~/.xemacs/line-num.el")
;;(load-file "~/.xemacs/linum.el")

;(require linum)

;;(linum-on)

;;(add-hook 'lua-mode-hook 'linum-on)
(column-number-mode t)
;; 关闭tool bar
;; (tool-bar-mode)

(defun home()
  (interactive)
  (find-file "~/"))
(defun msdos-to-unix-file()
  "make line ends with \n"
  (interactive)
  (goto-char (point-min))
  (while (search-forward "\r\n" nil t)
    (replace-match "\n")))

;; 设置paren的格式
;; (setq show-paren-style 'expression)
(setq show-paren-style 'parenthesis)

;; 命令解析
					; get rid of yes-or-no questions. y or n is enough
(defalias 'yes-or-no-p 'y-or-n-p)

					; shortening of often used commands
(defalias 'gf 'grep-find)
(defalias 'fd 'find-dired)
(defalias 'sh 'shell)

(defalias 'qrr 'query-replace-regexp)
(defalias 'lml 'list-matching-lines)
(defalias 'dml 'delete-matching-lines)
(defalias 'rof 'recentf-open-files)

(defalias 'eb 'eval-buffer)
(defalias 'er 'eval-region)
