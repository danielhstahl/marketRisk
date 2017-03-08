| [Linux][lin-link] | [Windows][win-link] | [Coveralls][cov-link] |
| :---------------: | :-----------------: | :-------------------: |
| ![lin-badge]      | ![win-badge]        | ![cov-badge]          |

[lin-badge]: https://travis-ci.org/phillyfan1138/marketRisk.svg?branch=master "Travis build status"
[lin-link]:  https://travis-ci.org/phillyfan1138/marketRisk "Travis build status"
[win-badge]: https://ci.appveyor.com/api/projects/status/5vfdsp398y70758w?svg=true "AppVeyor build status"
[win-link]:  https://ci.appveyor.com/project/phillyfan1138/marketrisk "AppVeyor build status"




Rewriting of market risk application.  Predominantly used for fixed income.  Assumes interest rate process is generalized Hull White.

Depends on HullWhite repository, Monte Carlo repository (histogram only), and FunctionalUtilities.  External repostiroy is rapidjson.

It depends on my <a href="https://github.com/phillyfan1138/HullWhite">Hull White</a>,  <a href="https://github.com/phillyfan1138/FunctionalUtilities">Functional Utilities</a>, 
<a href="https://github.com/phillyfan1138/MonteCarlo">Monte Carlo</a> repositories.  It also depends on <a href="https://github.com/miloyip/rapidjson">Rapidjson</a>.   It expects these repos to be cloned in the same directory as this repo.
