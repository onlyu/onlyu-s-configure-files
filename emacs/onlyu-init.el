(provide 'onlyu-init)

(require 'onlyu-help)
(require 'onlyu-fs)
(require 'onlyu-dict)

(require 'redo)

;; add .org file to agenda
(setq org-agenda-files (file-expand-wildcards "~/org/*.org"))

(add-to-list 'load-path
	     "~/emacs/yasnippet")
(require 'yasnippet) ;; not yasnippet-bundle
(yas/initialize)
(yas/load-directory "~/emacs/yasnippet/snippets")
(setq yas/prompt-functions '(yas/dropdown-prompt))

(require 'auto-complete-config)
(add-to-list 'ac-dictionary-directories "~/emacs/dict")
(ac-config-default)


(setq rj-ring-length 10000)
(require 'recent-jump)
;; lua-mode 
(require 'lua-mode)
(add-to-list 'auto-mode-alist '("\\.lua$" . lua-mode))
(autoload 'lua-mode "lua-mode" "Lua editing mode." t)


(require 'ido)
(ido-mode)

;; (iswitchb-mode)
(require 'anything)

;; flyspell-mode
(require 'flyspell)
(autoload 'flyspell-mode "flyspell" "On-the-fly spelling checker." t)
(autoload 'flyspell-delay-command "flyspell" "Delay on command." t)
(autoload 'tex-mode-flyspell-verify "flyspell" "" t)
(add-hook 'LaTeX-mode-hook 'flyspell-mode)
(add-hook 'lua-mode-hook 'flyspell-prog-mode)
(add-hook 'lisp-mode-hook 'flyspell-prog-mode)
(setq-default ispell-program-name "aspell")
(setq flyspell-issue-message-flag nil)

;; Support for marking a rectangle of text with highlighting.
(define-key ctl-x-map "r\C-@" 'rm-set-mark)
(define-key ctl-x-map [?r ?\C-\ ] 'rm-set-mark)
(define-key ctl-x-map "r\C-x" 'rm-exchange-point-and-mark)
(define-key ctl-x-map "r\C-w" 'rm-kill-region)
(define-key ctl-x-map "r\M-w" 'rm-kill-ring-save)
(define-key global-map [S-down-mouse-1] 'rm-mouse-drag-region)


(autoload 'rm-set-mark "rect-mark"
  "Set mark for rectangle." t)
(autoload 'rm-exchange-point-and-mark "rect-mark"
  "Exchange point and mark for rectangle." t)
(autoload 'rm-kill-region "rect-mark"
  "Kill a rectangular region and save it in the kill ring." t)
(autoload 'rm-kill-ring-save "rect-mark"
  "Copy a rectangular region to the kill ring." t)
(autoload 'rm-mouse-drag-region "rect-mark"
  "Drag out a rectangular region with the mouse." t)

;; for fs
(if (string= emacs-build-system "bogon")
    ;; for macosx settings
    (custom-set-variables
     '(fs-dir "~/fs/trunk/"))
  ;; for windows settings
  (custom-set-variables
   '(fs-dir "F:/fs/trunk/")))
;; cscope
(require 'xcscope)

;; key binddings
;; 涓嶄娇鐢╝lt閿墽琛屽懡浠�

(defun kill-line-or-region()
  (interactive)
  (let (pos1 pos2)
    (if (and transient-mark-mode mark-active)
	(progn
	  (setq pos1 (region-beginning) pos2 (region-end))
	  (kill-region pos1 pos2))
      (kill-line 1))
  ))

(global-set-key "\C-w" 'backward-kill-word)
(global-set-key "\M-=" 'redo)
(global-set-key "\M--" 'undo)
(global-set-key "\C-k" 'kill-line-or-region)

(global-set-key "\M-p" 'beginning-of-buffer)
(global-set-key "\M-n" 'end-of-buffer)

(global-set-key "\M-m" 'execute-extended-command)

;; other window use c-o
(global-set-key "\C-o" 'other-window)

;; use \C-i for backward delete char, do not use backspace or delete
;; because of portable problem
;;(global-set-key "\C-i" 'deletebackward-char)

;; for search
(global-set-key "\M-s" 'isearch-forward-regexp)
(global-set-key "\M-r" 'isearch-backward-regexp)
;;(global-set-key "\M-s" 'isearch-forward)
;;(global-set-key "\M-r" 'isearch-backward)
;;(global-set-key "\M-s" 'save-buffer)

;; make select region more effective
;; (global-set-key "\M- " 'set-mark-command)
(global-set-key [deletechar] 'delete-backward-char)
(global-set-key "\C-h" 'delete-backward-char)