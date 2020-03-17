from ruby:2.5.1

RUN apt-get update && apt-get install -y nodejs \
&& apt-get clean && rm -rf /var/lib/apt/lists/*
# RUN bundle config --global frozen 1

RUN mkdir /docs
WORKDIR /docs

# RUN gem install bundler
COPY Gemfile Gemfile.lock ./
RUN bundle install

COPY . /docs


ENTRYPOINT ./deploy.sh --source-only
