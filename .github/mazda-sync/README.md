# Mazda StarPilot upstream sync

`sync.sh` (run by `.github/workflows/mazda-starpilot-sync.yaml`) merges upstream StarPilot into a candidate branch,
fail-closed. It never auto-merges and never moves the anchor (`sync/state/upstream-anchor`) backwards.

## Upstream rewound / diverged

If the anchor is not an ancestor of the upstream head, the run fails (red). In `production` mode with `dry_run != true`
it also opens (or reuses) an issue labelled `sync-rewind`, titled
`Upstream StarPilot rewound/diverged: re-sync <patch branch> onto <upstream sha12>`, and assigns it to the Copilot coding
agent, which prepares a PR into the patch branch for human review. The issue body contains only SHAs, branch names and
fixed instructions (never upstream commit messages or authors). Other modes and dry runs make no writes.

### Secret setup

`GITHUB_TOKEN` cannot assign Copilot. Add a repository secret `COPILOT_ASSIGN_TOKEN`: a fine-grained PAT of the repo
owner with Issues, Pull requests and Contents write, with the Copilot coding agent enabled. If unset, the issue is still
created and the step summary says it must be assigned manually.

## Re-anchoring after merging the agent's PR

The anchor is never advanced automatically on a rewind. After merging the PR, run the workflow manually with
`mode=production` and `reanchor_sha=<full 40-char upstream head SHA>`. The SHA must equal the upstream head fetched by the
script and be reachable from it. The script force-updates only `sync/state/*` anchor branch and exits 0.
