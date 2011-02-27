(provide 'onlyu-init)

(require 'onlyu-help)
(require 'onlyu-fs)
(require 'onlyu-dict)

;;(global-set-key "\C-z" 'undo)
;;(global-set-key "\C-Z" 'redo)

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
(global-set-key (kbd "M-,") 'recent-jump-backward)
(global-set-key (kbd "M-.") 'recent-jump-forward)

(global-set-key (kbd "C-,") 'beginning-of-buffer)
(global-set-key (kbd "C-.") 'end-of-buffer)

;; lua-mode 
(require 'lua-mode)
(add-to-list 'auto-mode-alist '("\\.lua$" . lua-mode))
(autoload 'lua-mode "lua-mode" "Lua editing mode." t)

;; cedet 
;; (load-file "~/emacs/cedet/common/cedet.el")
;; (global-ede-mode 1)                      ; Enable the Project management system
;; (semantic-load-enable-code-helpers)      ; Enable prototype help and smart completion 
;; (global-srecode-minor-mode 1)            ; Enable template insertion menu

;; ecb
;; (add-to-list 'load-path "~/emacs/ecb/")
;; (require 'ecb-autoloads)


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
  (customize-set-variables
   '(fs-dir "F:/fs/trunk/")))
