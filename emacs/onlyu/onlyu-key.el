
;; windows key setting
(setq w32-pass-lwindow-to-system nil 
      w32-pass-rwindow-to-system nil 
      w32-pass-apps-to-system nil 
      w32-lwindow-modifier 'super ;; Left Windows key 
      w32-rwindow-modifier 'super ;; Right Windows key 
      w32-apps-modifier 'hyper) ;; Menu key

;; mac key setting
(setq mac-command-modifier 'nil)
(setq mac-option-modifier 'meta)
(setq mac-control-modifier 'ctl)

(provide 'onlyu-key)