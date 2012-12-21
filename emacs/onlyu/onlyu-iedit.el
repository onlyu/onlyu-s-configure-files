(require 'iedit)

(global-set-key "\C-cs" 'iedit-mode)

;;(define-key global-map (kbd "C-;") 'iedit-mode)
;;(define-key isearch-mode-map (kbd "C-;") 'iedit-mode)
(global-set-key "\C-ch" 'iedit-toggle-unmatched-lines-visible)

(provide 'onlyu-iedit)
