set -o errexit
set -o xtrace
cd ./site
git init
git config user.name "Travis CI"
git config user.email "deploy@travis-ci.org"
git add .
git commit -m "Automated Travis CI deployment"
git push --force "https://${GITHUB_TOKEN}@github.com/albertlauncher/albertlauncher.github.io.git" master # &> /dev/null
