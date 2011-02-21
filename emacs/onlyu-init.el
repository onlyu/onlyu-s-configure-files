(provide 'onlyu-init)

(require 'onlyu-help)
(require 'onlyu-fs)
(require 'redo)

;; add .org file to agenda
(setq org-agenda-files (file-expand-wildcards "~/org/*.org"))

(add-to-list 'load-path
	     "~/emacs/yasnippet")
(require 'yasnippet) ;; not yasnippet-bundle
(yas/initialize)
(yas/load-directory "~/emacs/yasnippet/snippets")