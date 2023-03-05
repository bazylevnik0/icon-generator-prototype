icon_generator_prototype_sources = [
  'main.c',
  'icon_generator_prototype-window.c',
  'icon_generator_prototype-application.c',
]

icon_generator_prototype_deps = [
  dependency('libadwaita-1', version: '>= 1.0'),
]

gnome = import('gnome')

icon_generator_prototype_sources += gnome.compile_resources('icon_generator_prototype-resources',
  'icon_generator_prototype.gresource.xml',
  c_name: 'icon_generator_prototype'
)

executable('icon-generator-prototype', icon_generator_prototype_sources,
  dependencies: icon_generator_prototype_deps,
  install: true,
)
