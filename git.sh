commit_message=$1
git_tag=$2

git add .

if [ -z "$commit_message" ]; then
  echo "Commit message is required"
  exit 1
fi

if [ -z "$git_tag" ]; then
  git commit -m "$commit_message"
  git push
  echo "Push Completed without tag: $commit_message"
else
  git commit -m "$commit_message"
  git push
  git tag "$git_tag"
  git push origin "$git_tag"
  echo "Push Completed with tag: $commit_message | $git_tag"
fi