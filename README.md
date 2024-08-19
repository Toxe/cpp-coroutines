# C++ Coroutines

C++ coroutine examples and library comparisons.

|  ns/op |           op/s | err% | total | benchmark                 |
| -----: | -------------: | ---: | ----: | :------------------------ |
| 347.17 |   2,880,445.80 | 3.5% |  0.01 | `cppcoro: simple_tasks`   |
| 436.46 |   2,291,147.13 | 1.1% |  0.01 | `libcoro: simple_tasks`   |
| 166.92 |   5,990,983.18 | 0.2% |  0.01 | `manual: simple_tasks`    |
|   1.20 | 835,937,051.56 | 2.4% |  0.01 | `reference: simple_tasks` |

|  ns/op |           op/s | err% | total | benchmark              |
| -----: | -------------: | ---: | ----: | :--------------------- |
| 154.23 |   6,484,003.01 | 1.2% |  0.01 | `cppcoro: fibonacci`   |
| 170.62 |   5,861,059.36 | 0.6% |  0.01 | `libcoro: fibonacci`   |
|  69.64 |  14,358,785.65 | 0.5% |  0.01 | `manual: fibonacci`    |
|   7.50 | 133,337,000.68 | 0.9% |  0.01 | `reference: fibonacci` |

|    ns/op |         op/s | err% | total | benchmark                                            |
| -------: | -----------: | ---: | ----: | :--------------------------------------------------- |
| 3,988.80 |   250,701.77 | 1.2% |  0.01 | `cppcoro generator: finite_counter (iterator)`       |
| 3,991.27 |   250,546.83 | 1.8% |  0.01 | `cppcoro generator: finite_counter (range-for)`      |
| 3,805.26 |   262,793.91 | 0.3% |  0.01 | `cppcoro generator: infinite_counter`                |
| 3,677.57 |   271,918.68 | 1.3% |  0.01 | `libcoro generator: finite_counter (iterator)`       |
| 3,745.11 |   267,014.66 | 2.0% |  0.01 | `libcoro generator: finite_counter (range-for)`      |
| 3,218.10 |   310,742.54 | 1.1% |  0.01 | `libcoro generator: infinite_counter`                |
| 3,141.80 |   318,288.55 | 0.4% |  0.01 | `manual generator: finite_counter (iterator)`        |
| 3,013.14 |   331,879.39 | 0.8% |  0.01 | `manual generator: finite_counter (range-for)`       |
| 3,327.68 |   300,509.34 | 1.5% |  0.01 | `manual generator: finite_counter`                   |
| 2,920.77 |   342,375.29 | 2.1% |  0.01 | `manual generator: infinite_counter`                 |
|   242.25 | 4,127,918.19 | 4.0% |  0.01 | `reference generator: finite_counter`                |
|   236.82 | 4,222,689.08 | 1.3% |  0.01 | `reference generator: infinite_counter`              |
| 3,680.00 |   271,739.13 | 0.9% |  0.01 | `std::generator: finite_counter (iterator)`          |
| 3,634.17 |   275,166.05 | 0.2% |  0.01 | `std::generator: finite_counter (range-for)`         |
| 3,277.56 |   305,104.64 | 0.1% |  0.01 | `std::generator: infinite_counter`                   |
| 3,669.04 |   272,550.92 | 2.0% |  0.01 | `concurrencpp generator: finite_counter (iterator)`  |
| 3,888.67 |   257,157.55 | 0.9% |  0.01 | `concurrencpp generator: finite_counter (range-for)` |
| 3,334.95 |   299,854.44 | 0.9% |  0.01 | `concurrencpp generator: infinite_counter`           |
