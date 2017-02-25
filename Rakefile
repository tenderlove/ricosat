# -*- ruby -*-

require 'rubygems'
require 'hoe'

Hoe.plugins.delete :rubyforge
Hoe.plugin :minitest
Hoe.plugin :gemspec # `gem install hoe-gemspec`
Hoe.plugin :git     # `gem install hoe-git`

Hoe.spec 'ricosat' do
  developer('Aaron Patterson', 'tenderlove@ruby-lang.org')
  license "MIT"
  self.readme_file   = 'README.md'
  self.history_file  = 'CHANGELOG.md'
  self.extra_rdoc_files  = FileList['*.md']

  self.spec_extras = {
    :extensions => ["ext/ricosat/extconf.rb"],
  }
  require 'rake/extensiontask'
  Rake::ExtensionTask.new("ricosat") do |ext|
    ext.lib_dir = File.join(*['lib', ENV['FAT_DIR']].compact)
  end
end

# vim: syntax=ruby
