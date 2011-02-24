(provide 'onlyu-init)

(require 'onlyu-help)
(require 'onlyu-fs)

(global-set-key "\C-z" 'undo)
(global-set-key "\C-Z" 'redo)

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
(load-file "~/emacs/cedet/common/cedet.el")
(global-ede-mode 1)                      ; Enable the Project management system
(semantic-load-enable-code-helpers)      ; Enable prototype help and smart completion 
(global-srecode-minor-mode 1)            ; Enable template insertion menu

;; ecb
(add-to-list 'load-path "~/emacs/ecb/")
(require 'ecb-autoloads)